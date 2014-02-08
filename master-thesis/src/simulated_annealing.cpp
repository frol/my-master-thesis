bool SimulatedAnnealing::run(SBox &sbox, int MIL, int MaxIL,
                             int MUL, double alpha)
{
    this->init(sbox);

    SBox best_solution(sbox);
    SBox temp_F(sbox);
    best_solution.set(sbox);
    long double cost = get_cost(sbox);
    long double temp_cost, cost_delta;
    int NL_start = sbox.get_NL();
    int AC_start = sbox.get_AC();
    int temp_NL, temp_AC;

    double T = this->T0;
    int MFC = 0;
    temp_F.set(sbox);
    for (int i = 0; i < MaxIL; ++i)
    {
        bool changed = false;
        for (int j = 0; j < MIL; ++j)
        {
            generate_neighbor(temp_F);
            temp_cost = get_cost(temp_F);

            cost_delta = temp_cost - cost;
            bool accepted = false;
            if (cost_delta < 0)
                accepted = true;
            else
            {
                double U = double(rand()) / double(RAND_MAX);
                if (U < exp(double(-cost_delta / T)))
                    accepted = true;
                else
                    restore_generated_neighbor(temp_F);
            }
            if (accepted)
            {
                changed = true;
                cost = temp_cost;
                temp_NL = temp_F.get_NL();
                temp_AC = temp_F.get_AC();
                #ifdef ENABLED_WISHED_BREAK
                if ((temp_NL >= WISHED_NL) && (temp_AC <= WISHED_AC))
                    #ifdef DES_CRITERIA_IN_ANNEALING
                    if (testDES(temp_F.F))
                    #endif
                {
                    sbox.set(temp_F);
                    return true;
                }
                #endif
                if ((NL_start < temp_NL) || ((NL_start == temp_NL)
                                            && (AC_start > temp_AC)))
                {
                    best_solution.set(temp_F);
                    NL_start = temp_NL;
                    AC_start = temp_AC;
                }
            }

            ++this->pos_index;
        }//for (MIL)
        if (!changed)
            ++MFC;
        else
            MFC = 0;
        if (MFC == MUL)
            break;
        T *= alpha;
    }//for (MaxIL)
    sbox.set(best_solution);
    return false;
}
