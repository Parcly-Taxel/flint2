/*
    Copyright (C) 2015 Vladimir Glazachev

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "flint.h"
#include "aprcl.h"

int main(void)
{
    int i, j;
    FLINT_TEST_INIT(state);
   
    flint_printf("unity_zpq_equal....");
    fflush(stdout);

    for (i = 0; i < 10 * flint_test_multiplier(); i++)
    {
        ulong p, q;
        fmpz_t n;
        unity_zpq f, g;

        p = n_randprime(state, 2 + n_randint(state, 6), 0);
        q = n_randprime(state, 2 + n_randint(state, 6), 0);

        fmpz_init(n);
        fmpz_randtest_unsigned(n, state, 200);
        while (fmpz_equal_ui(n, 0) != 0)
            fmpz_randtest_unsigned(n, state, 200);

        unity_zpq_init(f, q, p, n);
        unity_zpq_init(g, q, p, n);

        for (j = 0; j < 100; j++)
        {
            ulong x, y;
            fmpz_t val;

            fmpz_init(val);

            x = n_randint(state, p);
            y = n_randint(state, q);

            fmpz_randtest_not_zero(val, state, 300);
            unity_zpq_coeff_set_fmpz(f, y, x, val);
            unity_zpq_coeff_set_fmpz(g, y, x, val);

            fmpz_clear(val);
        }

        if (unity_zpq_equal(f, g) == 0)
        {
            flint_printf("FAIL\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_clear(n);
        unity_zpq_clear(f);
        unity_zpq_clear(g);
    }

    FLINT_TEST_CLEANUP(state);
    
    flint_printf("PASS\n");
    return 0;
}

