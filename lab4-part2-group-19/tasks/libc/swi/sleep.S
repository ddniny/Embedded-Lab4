/** @file sleep.S
 *
 * @brief sleep sycall wrapper
 *
 * @author Danni Wu <danniw@andrew.cmu.edu>
 *         Yu Qi <yq@andrew.cmu.edu>
 *         Wenting Gong <wentingg@andrew.cmu.edu>
 * @date 2013-11-20
 */

#include <asm.h>
#include <bits/swi.h>

	.file "sleep.S"

FUNC(sleep)
	@ Add code here.  Put your names at the top.
	SWI	SLEEP_SWI
	MOV	pc, lr





