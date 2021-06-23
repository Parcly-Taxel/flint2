/*
    Copyright (C) 2021 Albin Ahlbäck

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include "flint.h"
#include "ulong_extras.h"
#include "fmpz.h"

int
main(void)
{
    int i, result;
    FLINT_TEST_INIT(state);

    flint_printf("ndiv_qr....");
    fflush(stdout);

    

    for (i = 0; i < 10000 * flint_test_multiplier(); i++)
    {
        fmpz_t max;
        fmpz_t tmp;
        fmpz_t a, b;
        fmpz_t nquo, nrem;
        fmpz_t fquo, frem;
        fmpz_t cquo, crem;

        fmpz_init(max);
        fmpz_init(tmp);
        fmpz_init(a);
        fmpz_init(b);
        fmpz_init(nquo);
        fmpz_init(nrem);
        fmpz_init(fquo);
        fmpz_init(frem);
        fmpz_init(cquo);
        fmpz_init(crem);

        fmpz_set_d_2exp(max, 1.0, FLINT_BITS);
        fmpz_randm(a, state, max);
        fmpz_randm(b, state, max);
        if (n_randint(state, 2))
            fmpz_neg(a, a);
        if (n_randint(state, 2))
            fmpz_neg(b, b);
        if (fmpz_is_zero(b))
            fmpz_one(b);

        fmpz_ndiv_qr(nquo, nrem, a, b);
        fmpz_fdiv_qr(fquo, frem, a, b);
        fmpz_cdiv_qr(cquo, crem, a, b);

        /* Check that a = b * nquo + nrem, and tha nrem is smallest */
        fmpz_set(tmp, nrem);
        fmpz_addmul(tmp, b, nquo);
        result = ( fmpz_cmp(tmp, a) == 0
                && fmpz_cmpabs(nrem, frem) <= 0
                && fmpz_cmpabs(nrem, crem) <= 0);
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("a = "); fmpz_print(a); flint_printf("\n");
            flint_printf("b = "); fmpz_print(b); flint_printf("\n");
            flint_printf("q = "); fmpz_print(nquo); flint_printf("\n");
            flint_printf("r = "); fmpz_print(nrem); flint_printf("\n");
            flint_printf("a is big? %d\n", COEFF_IS_MPZ(*a));
            flint_printf("b is big? %d\n", COEFF_IS_MPZ(*b));
            abort();
        }

        fmpz_clear(max);
        fmpz_clear(tmp);
        fmpz_clear(a);
        fmpz_clear(b);
        fmpz_clear(nquo);
        fmpz_clear(nrem);
        fmpz_clear(fquo);
        fmpz_clear(frem);
        fmpz_clear(cquo);
        fmpz_clear(crem);
    }

    FLINT_TEST_CLEANUP(state);
    
    flint_printf("PASS\n");
    return 0;
}
