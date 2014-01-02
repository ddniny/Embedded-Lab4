/** @file crt0.S
 *
 * @brief Execution startup routine
 *
 * @author Danni Wu <danniw@andrew.cmu.edu>
 * @	   Yu Qi <yq@andrew.cmu.edu>
 * @	   Wenting Gong <wentingg@andrew.cmu.edu>
 *
 * @date   11/20/2013
 */

#include <asm.h>

	.file "crt0.S"

FUNC(_start)
	@ needs code.  Put your names at the top.
loop:
	BL   main	@call function main with the first argument in r0 and the second argument in r1
	B    loop	@crt0 should go into an infinite loop if main returns.
