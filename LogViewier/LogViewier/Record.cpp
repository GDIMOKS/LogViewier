#include "Record.h"


void Record::calculate_standard_features(vector<Frame>& frames)
{
    if (frames.empty())
        return;

    vector<double> frameSizes;
    vector<double> frameOffsets;

    for (Frame f : frames)
    {
        if (f.getCorrect())
        {
            double size = stod(f.getSize());
            double time = stod(f.getOffset());

            frameSizes.push_back(size);
            frameOffsets.push_back(time);
        }
    }

    Features sizeFeatures = Features(frameSizes);
    Features timeFeatures = Features(frameOffsets);

    add_standard_features_to_all(sizeFeatures);
    add_standard_features_to_all(timeFeatures);

    cout << "Standard features:" << endl;
    print_standard_features(sizeFeatures, timeFeatures);
}

void Record::add_standard_features_to_all(Features& standard_features)
{
    features.push_back(standard_features.getStDeviation());
    features.push_back(standard_features.getVariance());
    features.push_back(standard_features.getRootMSq());
    features.push_back(standard_features.getMeanSq());
    features.push_back(standard_features.getPSkewness());
    features.push_back(standard_features.getKyrtosis());
    features.push_back(standard_features.getSkewness());
    features.push_back(standard_features.getMinValue());
    features.push_back(standard_features.getMaxValue());
    features.push_back(standard_features.getMean());
    features.push_back(standard_features.getMedian());
    features.push_back(standard_features.getMedianAD());
}

vector<double> Record::get_features()
{
    return features;
}

void Record::set_features(vector<Frame>& frames)
{
    calculate_unique_features(frames);
    calculate_standard_features(frames);
}

void Record::print_standard_features(Features& sizeFeatures, Features& timeFeatures)
{
    cout.width(26);
    cout << "\t" << setw(28) << "Sizes" << setw(30) << "Offsets" << endl;
    cout << "\t" << setw(25) << "standard deviation:" << setw(28) << sizeFeatures.getStDeviation() << timeFeatures.getStDeviation() << endl;
    cout << "\t" << setw(25) << "variance:" << setw(28) << sizeFeatures.getVariance() << timeFeatures.getVariance() << endl;
    cout << "\t" << setw(25) << "root mean square:" << setw(28) << sizeFeatures.getRootMSq() << timeFeatures.getRootMSq() << endl;
    cout << "\t" << setw(25) << "m_square:" << setw(28) << sizeFeatures.getMeanSq() << timeFeatures.getMeanSq() << endl;
    cout << "\t" << setw(25) << "p_skewness:" << setw(28) << sizeFeatures.getPSkewness() << timeFeatures.getPSkewness() << endl;
    cout << "\t" << setw(25) << "kyrtosys:" << setw(28) << sizeFeatures.getKyrtosis() << timeFeatures.getKyrtosis() << endl;
    cout << "\t" << setw(25) << "skewness:" << setw(28) << sizeFeatures.getSkewness() << timeFeatures.getSkewness() << endl;
    cout << "\t" << setw(25) << "min:" << setw(28) << sizeFeatures.getMinValue() << timeFeatures.getMinValue() << endl;
    cout << "\t" << setw(25) << "max:" << setw(28) << sizeFeatures.getMaxValue() << timeFeatures.getMaxValue() << endl;
    cout << "\t" << setw(25) << "mean:" << setw(28) << sizeFeatures.getMean() << timeFeatures.getMean() << endl;
    cout << "\t" << setw(25) << "median:" << setw(28) << sizeFeatures.getMedian() << timeFeatures.getMedian() << endl;
    cout << "\t" << setw(25) << "medianAD:" << setw(28) << sizeFeatures.getMedianAD() << timeFeatures.getMedianAD() << endl;
}

void Record::calculate_unique_features(vector<Frame>& frames)
{
    if (frames.size() < 20)
        return;

    float mtu_size = -1;
    vector<float> pivots;
    float pivot_size = -1;
    float sum = 0;
    float total_sample_size = 0;
    vector<Frame> buffer;
    int j = 0;

    int fingerprints[3] = { 0 };


    for (int i = 0; i < 20; i++)
    {
        sum += stof(frames[i].getSize());

        switch (buffer.size())
        {
        case 5: // MLLLM MLLLL

            if (buffer.front().getSize() == buffer.back().getSize())
            {
                pivots.push_back(stof(buffer[buffer.size() - 2].getSize())); // L - pivot
                buffer.erase(buffer.begin(), buffer.end() - 1);         // MLLLM -> M
                fingerprints[2]++;
            }
            else
                buffer.erase(buffer.begin(), buffer.end() - 1);         // MLLLL -> L

            break;
        case 4: // MLLL MLLM
            if (stof(buffer.front().getSize()) == mtu_size && stof(buffer.back().getSize()) == mtu_size)
            {
                pivots.push_back(stof(buffer[buffer.size() - 2].getSize())); // L - pivot
                buffer.erase(buffer.begin(), buffer.end() - 1);         // MLLM -> M
                fingerprints[1]++;
            }
            break;
        case 3:
            if (stof(buffer.front().getSize()) == mtu_size && stof(buffer.back().getSize()) == mtu_size) // MMM MLM
            {
                if (buffer[1].getSize() != buffer[0].getSize()) // L - pivot, MLM -> M
                {
                    pivots.push_back(stof(buffer[buffer.size() - 2].getSize()));
                    fingerprints[0]++;
                }

                buffer.erase(buffer.begin(), buffer.end() - 1); // MMM -> M
            }
            else if (stof(buffer[1].getSize()) == mtu_size || stof(buffer[0].getSize()) == mtu_size)// LMM LML MML MLL
            {
                if (buffer[0].getSize() != buffer[1].getSize()) // MLL
                    break;

                buffer.erase(buffer.begin()); // LMM LML MML -> MM ML 
            }
            else // LLL LLM
                buffer.erase(buffer.begin(), buffer.end() - 1); // L M

            break;
        }

        buffer.push_back(frames[i]);
        if (buffer.size() > 5)
            buffer.erase(buffer.begin());

        if (stof(frames[i].getSize()) > mtu_size)
        {
            mtu_size = stof(frames[i].getSize());
        }
    }

    if (!pivots.empty())
    {
        int popularity = 0;

        for (float pivot : pivots)
        {
            int pivot_popularity = count(pivots.begin(), pivots.end(), pivot);
            if (pivot_popularity > popularity)
            {
                popularity = pivot_popularity;
                pivot_size = pivot;
            }
        }
    }

    total_sample_size = sum;

    features.push_back(pivot_size);
    features.push_back(pivot_size / mtu_size);
    features.push_back(pivot_size / total_sample_size);

    cout << "-------------------------------------------------------------------------------" << endl;
    cout << "Unique features:" << endl;
    print_unique_features(pivot_size, mtu_size, total_sample_size, fingerprints);

}

void Record::print_unique_features(float pivot_size, float mtu_size, float total_sample_size, int* fingerprints)
{
    cout  << left << "\t" << setw(15) << "mtu_size:" << mtu_size << endl;
    if (pivot_size == -1)
    {
        cout << "\t" << setw(15) << "pivot_size:" << "not udentified" << endl;
        return;
    }

    cout << "\t" << setw(15) << "pivot_size:" << pivot_size << endl;
    //cout << "\t" << setw(15) << "total_sample_size: " << total_sample_size << endl;
    cout << "\t" << setw(15) << "PM:" << pivot_size / mtu_size << endl;
    cout << "\t" << setw(15) << "PT:" << pivot_size / total_sample_size << endl;

    int fingerprints_sum = fingerprints[0] + fingerprints[1] + fingerprints[2];
    cout << "\npopularity of fingerprints:" << endl;
    cout << "\t" << setw(15) << "sum:" << fingerprints_sum << endl;
    cout << "\t" << setw(15) << "1st (MLM):" << fingerprints[0] << endl;
    cout << "\t" << setw(15) << "2nd (MLLM):" << fingerprints[1] << endl;
    cout << "\t" << setw(15) << "3rd (MLLLM):" << fingerprints[2] << endl;
}