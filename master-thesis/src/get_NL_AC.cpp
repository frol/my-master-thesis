int SBox::get_NL()
{
    int WHT_max = 0;
    for (int i = 0; i < this->output_combinations - 1; ++i)
        for (int w = 0; w < this->input_combinations; ++w)
        {
            int sum = 0;
            for (int x = 0; x < this->input_combinations; ++x)
                sum += (boolean_f[i][x] ^ __builtin_popcount(w & x) & 0x1)
                        ? -1 : 1;
            if (sum < 0)
                sum = -sum;
            if (sum > WHT_max)
                WHT_max = sum;
        }
    return (this->input_combinations - WHT_max) >> 1;
}

int SBox::get_AC()
{
    int max = 0;
    for (int i = 0; i < this->output_combinations - 1; ++i)
        for (int delta = 1; delta < this->input_combinations; ++delta)
        {
            int sum = 0;
            for (int x = 0; x < this->input_combinations; ++x)
                sum += (boolean_f[i][x] ^ boolean_f[i][x ^ delta]) ? -1 : 1;
            if (sum < 0)
                sum = -sum;
            if (sum > max)
                max = sum;
        }
    return max;
}
