/*
    Copyright (C) 2010 William Hart

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
#include "nmod_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, j, result;
    FLINT_TEST_INIT(state);
    

    flint_printf("pow_binexp....");
    fflush(stdout);

    /* Check powering against naive method */
    for (i = 0; i < 1000 * flint_test_multiplier(); i++)
    {
        nmod_poly_t a, b, c;
        mp_limb_t n = n_randtest_not_zero(state);
        slong e;

        nmod_poly_init(a, n);
        nmod_poly_init(b, n);
        nmod_poly_init(c, n);
        nmod_poly_randtest(a, state, n_randint(state, 30));
        e = n_randint(state, 20);

        nmod_poly_pow_binexp(b, a, e);
        
        nmod_poly_set_coeff_ui(c, 0, 1);
        for (j = 0; j < e; j++)
            nmod_poly_mul(c, c, a);

        result = (nmod_poly_equal(b, c) 
            || (a->length == 0 && e == 0 && c->length == 1 && c->coeffs[0] == 1));
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("a->length = %wd, n = %wu, exp = %wd\n", a->length, a->mod.n, e);
            nmod_poly_print(a), flint_printf("\n\n");
            nmod_poly_print(b), flint_printf("\n\n");
            nmod_poly_print(c), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        nmod_poly_clear(a);
        nmod_poly_clear(b);
        nmod_poly_clear(c);
    }

    /* Check aliasing */
    for (i = 0; i < 1000 * flint_test_multiplier(); i++)
    {
        nmod_poly_t a, b, c;
        mp_limb_t n = n_randtest_not_zero(state);
        slong e;

        nmod_poly_init(a, n);
        nmod_poly_init(b, n);
        nmod_poly_init(c, n);
        nmod_poly_randtest(a, state, n_randint(state, 30));
        e = n_randint(state, 20);

        nmod_poly_pow_binexp(b, a, e);
        
        nmod_poly_set(c, a);
        nmod_poly_pow_binexp(c, c, e);
        
        result = (nmod_poly_equal(b, c));
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("a->length = %wd, n = %wu, exp = %wd\n", a->length, a->mod.n, e);
            nmod_poly_print(a), flint_printf("\n\n");
            nmod_poly_print(b), flint_printf("\n\n");
            nmod_poly_print(c), flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        nmod_poly_clear(a);
        nmod_poly_clear(b);
        nmod_poly_clear(c);
    }

    FLINT_TEST_CLEANUP(state);
    
    flint_printf("PASS\n");
    return 0;
}
