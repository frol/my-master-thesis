void SBox::get_MD_args(int& d, int& d_)
{
    d = 0;
    d_ = 0;
    for (int alpha = 1; alpha < this->output_combinations; ++alpha)
        for (int beta = 1; beta < this->output_combinations; ++beta)
        {
            int sum_d_a0 = 0;
            int sum_d_a1 = 0;
            for (int k = 0; k < this->output_combinations; ++k)
            {
                // delta(a, b) == (1 if a == b else 0)
                if ((this->F[S_index((k + alpha) & (this->output_combinations - 1))] ^ F[S_index(k)]) == beta)
                    // v(k, alpha) is the carry bit
                    if (k + alpha >= this->output_combinations)
                        ++sum_d_a1;
                    else
                        ++sum_d_a0;
            }
            if (sum_d_a0 > d_)
                d_ = sum_d_a0;
            if (sum_d_a1 > d_)
                d_ = sum_d_a1;
            if (sum_d_a0 + sum_d_a1 > d)
                d = sum_d_a0 + sum_d_a1;
        }
}

double SBox::get_MD(int cipher_round)
{
    int d, d_;
    this->get_MD_args(d, d_);
    if (cipher_round == 0)
        cipher_round = this->cipher_rounds_count;
    return std::max(
        (
            pow(double(d) / this->output_combinations, cipher_round + 1 - 2 * ceil(cipher_round / 3.0))
            * pow(double(d_) / this->output_combinations, ceil(cipher_round / 3.0))
        ),
        pow(double(d) / this->output_combinations, cipher_round - 1)
    );
}

void SBox::get_ML_args(int& l)
{
    l = 0;
    for (int alpha = 1; alpha < this->output_combinations; ++alpha)
        for (int beta = 1; beta < this->output_combinations; ++beta)
        {
            int k_sum = 0;
            for (int k = 0; k < this->output_combinations; ++k)
            {
                int x_sum = 0;
                for (int x = 0; x < this->output_combinations; ++x)
                    x_sum += ((__builtin_popcount(beta & this->F[S_index((x + k) & (this->output_combinations - 1))]) ^ __builtin_popcount(alpha & x)) & 0x1) ? -1 : 1;
                k_sum += x_sum * x_sum;
            }
            if (k_sum > l)
                l = k_sum;
        }
}

double SBox::get_ML(int cipher_round)
{
    int l;
    this->get_ML_args(l);
    if (cipher_round == 0)
        cipher_round = this->cipher_rounds_count;
    return pow(
        double(l) / this->output_combinations / this->output_combinations / this->output_combinations,
        round(2.0 / 3 * cipher_round)
    );
}
