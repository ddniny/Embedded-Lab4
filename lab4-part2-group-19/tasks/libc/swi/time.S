/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Danni Wu <danniw@andrew.cmu.edu>
 *         Yu Qi <yq@andrew.cmu.edu>
 *         Wenting Gong <wentingg@andrew.cmu.edu>
 * @date 2013-11-20
 */

#include <asm.h>
#include <bits/swi.h>

	.file "time.S"

FUNC(time)
	@ Add code here.  Put your name at the top.
	swi TIME_SWI		@go to 0x08
	mov pc,lr			@return

