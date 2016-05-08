/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1999, 2000 by Ralf Baechle
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 */
#ifndef _ASM_SPINLOCK_H
#define _ASM_SPINLOCK_H

#include <asm/war.h>

/*
 * Your basic SMP spinlocks, allowing only a single CPU anywhere
 */

#define __raw_spin_is_locked(x)       ((x)->lock != 0)
#define __raw_spin_lock_flags(lock, flags) __raw_spin_lock(lock)
#define __raw_spin_unlock_wait(x) \
	do { cpu_relax(); } while ((x)->lock)

/*
 * Simple spin lock operations.  There are two variants, one clears IRQ's
 * on the local processor, one does not.
 *
 * We make no fairness assumptions.  They have a cost.
 */

static inline void __raw_spin_lock(raw_spinlock_t *lock)
{
	unsigned int tmp;

	if (R10000_LLSC_WAR) {
		__asm__ __volatile__(
		"	.set	noreorder	# __raw_spin_lock	\n"
		"1:	ll	%1, %2					\n"
		"	bnez	%1, 1b					\n"
		"	 li	%1, 1					\n"
		"	sc	%1, %0					\n"
		"	beqzl	%1, 1b					\n"
		"	 nop						\n"
		"	sync						\n"
		"	.set	reorder					\n"
		: "=m" (lock->lock), "=&r" (tmp)
		: "m" (lock->lock)
		: "memory");
	} else {
#if defined(CONFIG_CPU_CAVIUM_OCTEON) && defined(__OCTEON__)
        /* When using the Octeon compiler, use syncw instead of sync since
            it is faster */
		__asm__ __volatile__(
		"	.set	noreorder	# _raw_spin_lock	\n"
		"	 syncw				# Flush the write buffer while we're waiting\n"
		"1:	ll	%1, %2					\n"
		"	bnez	%1, 1b					\n"
		"	li	%1, 1					\n"
		"	sc	%1, %0			# This will go out after the syncw completes\n"
		"	beqz	%1, 1b					\n"
		"	 nop # No syncw is needed here since the sc goes out immediately\n"
		"	.set	reorder					\n"
		: "=m" (lock->lock), "=&r" (tmp)
		: "m" (lock->lock)
		: "memory");
#else
		__asm__ __volatile__(
		"	.set	noreorder	# __raw_spin_lock	\n"
		"1:	ll	%1, %2					\n"
		"	bnez	%1, 1b					\n"
		"	 li	%1, 1					\n"
		"	sc	%1, %0					\n"
		"	beqz	%1, 1b					\n"
		"	 sync						\n"
		"	.set	reorder					\n"
		: "=m" (lock->lock), "=&r" (tmp)
		: "m" (lock->lock)
		: "memory");
#endif
	}
}

static inline void __raw_spin_unlock(raw_spinlock_t *lock)
{
	__asm__ __volatile__(
	"	.set	noreorder	# __raw_spin_unlock	\n"
#if defined(CONFIG_CPU_CAVIUM_OCTEON) && defined(__OCTEON__)
        /* When using the Octeon compiler, use syncw instead of sync since
            it is faster */
	"	syncw   # Make sure the unlock doesn't merge with other writes\n"
#else
	"	sync						\n"
#endif
	"	sw	$0, %0					\n"
#if defined(CONFIG_CPU_CAVIUM_OCTEON) && defined(__OCTEON__)
    /* This is an optimization to make sure the unlock doesn't sit in the write buffer */
	"	syncw   # Force the unlock write out\n"
#endif
	"	.set\treorder					\n"
	: "=m" (lock->lock)
	: "m" (lock->lock)
	: "memory");
}

static inline unsigned int __raw_spin_trylock(raw_spinlock_t *lock)
{
	unsigned int temp, res;

	if (R10000_LLSC_WAR) {
		__asm__ __volatile__(
		"	.set	noreorder	# __raw_spin_trylock	\n"
		"1:	ll	%0, %3					\n"
		"	ori	%2, %0, 1				\n"
		"	sc	%2, %1					\n"
		"	beqzl	%2, 1b					\n"
		"	 nop						\n"
		"	andi	%2, %0, 1				\n"
		"	sync						\n"
		"	.set	reorder"
		: "=&r" (temp), "=m" (lock->lock), "=&r" (res)
		: "m" (lock->lock)
		: "memory");
	} else {
#if defined(CONFIG_CPU_CAVIUM_OCTEON) && defined(__OCTEON__)
        /* When using the Octeon compiler, use syncw instead of sync since
            it is faster */
		__asm__ __volatile__(
		"	.set	noreorder	# _raw_spin_trylock	\n"
		"	syncw				# Flush the write buffer while we're waiting\n"
		"1:	ll	%0, %3					\n"
		"	ori	%2, %0, 1				\n"
		"	sc	%2, %1			# This will go out after the syncw completes\n"
		"	beqz	%2, 1b					\n"
		"	 andi	%2, %0, 1				\n"
		"	# No syncw is needed here since the sc goes out immediately\n"
		"	.set	reorder"
		: "=&r" (temp), "=m" (lock->lock), "=&r" (res)
		: "m" (lock->lock)
		: "memory");
#else
		__asm__ __volatile__(
		"	.set	noreorder	# __raw_spin_trylock	\n"
		"1:	ll	%0, %3					\n"
		"	ori	%2, %0, 1				\n"
		"	sc	%2, %1					\n"
		"	beqz	%2, 1b					\n"
		"	 andi	%2, %0, 1				\n"
		"	sync						\n"
		"	.set	reorder"
		: "=&r" (temp), "=m" (lock->lock), "=&r" (res)
		: "m" (lock->lock)
		: "memory");
#endif
	}

	return res == 0;
}

/*
 * Read-write spinlocks, allowing multiple readers but only one writer.
 *
 * NOTE! it is quite common to have readers in interrupts but no interrupt
 * writers. For those circumstances we can "mix" irq-safe locks - any writer
 * needs to get a irq-safe write-lock, but readers can get non-irqsafe
 * read-locks.
 */

/*
 * read_can_lock - would read_trylock() succeed?
 * @lock: the rwlock in question.
 */
#define __raw_read_can_lock(rw)	((rw)->lock >= 0)

/*
 * write_can_lock - would write_trylock() succeed?
 * @lock: the rwlock in question.
 */
#define __raw_write_can_lock(rw)	(!(rw)->lock)

static inline void __raw_read_lock(raw_rwlock_t *rw)
{
	unsigned int tmp;

	if (R10000_LLSC_WAR) {
		__asm__ __volatile__(
		"	.set	noreorder	# __raw_read_lock	\n"
		"1:	ll	%1, %2					\n"
		"	bltz	%1, 1b					\n"
		"	 addu	%1, 1					\n"
		"	sc	%1, %0					\n"
		"	beqzl	%1, 1b					\n"
		"	 nop						\n"
		"	sync						\n"
		"	.set	reorder					\n"
		: "=m" (rw->lock), "=&r" (tmp)
		: "m" (rw->lock)
		: "memory");
	} else {
#if defined(CONFIG_CPU_CAVIUM_OCTEON) && defined(__OCTEON__)
        /* When using the Octeon compiler, use syncw instead of sync since
            it is faster */
		__asm__ __volatile__(
		"	.set	noreorder	# _raw_read_lock	\n"
		"	syncw				# Flush the write buffer while we're waiting\n"
		"1:	ll	%1, %2					\n"
		"	bltz	%1, 1b					\n"
		"	 addu	%1, 1					\n"
		"	sc	%1, %0			# This will go out after the syncw completes\n"
		"	beqz	%1, 1b					\n"
		"	 nop # No syncw is needed here since the sc goes out immediately\n"
		"	.set	reorder					\n"
		: "=m" (rw->lock), "=&r" (tmp)
		: "m" (rw->lock)
		: "memory");
#else
		__asm__ __volatile__(
		"	.set	noreorder	# __raw_read_lock	\n"
		"1:	ll	%1, %2					\n"
		"	bltz	%1, 1b					\n"
		"	 addu	%1, 1					\n"
		"	sc	%1, %0					\n"
		"	beqz	%1, 1b					\n"
		"	 sync						\n"
		"	.set	reorder					\n"
		: "=m" (rw->lock), "=&r" (tmp)
		: "m" (rw->lock)
		: "memory");
#endif
	}
}

/* Note the use of sub, not subu which will make the kernel die with an
   overflow exception if we ever try to unlock an rwlock that is already
   unlocked or is being held by a writer.  */
static inline void __raw_read_unlock(raw_rwlock_t *rw)
{
	unsigned int tmp;

	if (R10000_LLSC_WAR) {
		__asm__ __volatile__(
		"1:	ll	%1, %2		# __raw_read_unlock	\n"
		"	sub	%1, 1					\n"
		"	sc	%1, %0					\n"
		"	beqzl	%1, 1b					\n"
		"	sync						\n"
		: "=m" (rw->lock), "=&r" (tmp)
		: "m" (rw->lock)
		: "memory");
	} else {
#if defined(CONFIG_CPU_CAVIUM_OCTEON) && defined(__OCTEON__)
        /* When using the Octeon compiler, use syncw instead of sync since
            it is faster */
		__asm__ __volatile__(
		"	.set	noreorder	# _raw_read_unlock	\n"
		"	syncw				# Flush the write buffer while we're waiting\n"
		"1:	ll	%1, %2					\n"
		"	sub	%1, 1					\n"
		"	sc	%1, %0			# This will go out after the syncw completes\n"
		"	beqz	%1, 1b					\n"
		"	 nop # No syncw is needed here since the sc goes out immediately\n"
		"	.set	reorder					\n"
		: "=m" (rw->lock), "=&r" (tmp)
		: "m" (rw->lock)
		: "memory");
#else
		__asm__ __volatile__(
		"	.set	noreorder	# __raw_read_unlock	\n"
		"1:	ll	%1, %2					\n"
		"	sub	%1, 1					\n"
		"	sc	%1, %0					\n"
		"	beqz	%1, 1b					\n"
		"	 sync						\n"
		"	.set	reorder					\n"
		: "=m" (rw->lock), "=&r" (tmp)
		: "m" (rw->lock)
		: "memory");
#endif
	}
}

static inline void __raw_write_lock(raw_rwlock_t *rw)
{
	unsigned int tmp;

	if (R10000_LLSC_WAR) {
		__asm__ __volatile__(
		"	.set	noreorder	# __raw_write_lock	\n"
		"1:	ll	%1, %2					\n"
		"	bnez	%1, 1b					\n"
		"	 lui	%1, 0x8000				\n"
		"	sc	%1, %0					\n"
		"	beqzl	%1, 1b					\n"
		"	 sync						\n"
		"	.set	reorder					\n"
		: "=m" (rw->lock), "=&r" (tmp)
		: "m" (rw->lock)
		: "memory");
	} else {
#if defined(CONFIG_CPU_CAVIUM_OCTEON) && defined(__OCTEON__)
        /* When using the Octeon compiler, use syncw instead of sync since
            it is faster */
		__asm__ __volatile__(
		"	.set	noreorder	# _raw_write_lock	\n"
		"	syncw				# Flush the write buffer while we're waiting\n"
		"1:	ll	%1, %2					\n"
		"	bnez	%1, 1b					\n"
		"	 lui	%1, 0x8000				\n"
		"	sc	%1, %0			# This will go out after the syncw completes\n"
		"	beqz	%1, 1b					\n"
		"	 nop # No syncw is needed here since the sc goes out immediately\n"
		"	.set	reorder					\n"
		: "=m" (rw->lock), "=&r" (tmp)
		: "m" (rw->lock)
		: "memory");
#else
		__asm__ __volatile__(
		"	.set	noreorder	# __raw_write_lock	\n"
		"1:	ll	%1, %2					\n"
		"	bnez	%1, 1b					\n"
		"	 lui	%1, 0x8000				\n"
		"	sc	%1, %0					\n"
		"	beqz	%1, 1b					\n"
		"	 sync						\n"
		"	.set	reorder					\n"
		: "=m" (rw->lock), "=&r" (tmp)
		: "m" (rw->lock)
		: "memory");
#endif
	}
}

static inline void __raw_write_unlock(raw_rwlock_t *rw)
{
#if defined(CONFIG_CPU_CAVIUM_OCTEON) && defined(__OCTEON__)
        /* When using the Octeon compiler, use syncw instead of sync since
            it is faster */
	__asm__ __volatile__(
	"	syncw			# __raw_write_unlock	\n"
	"	sw	$0, %0					\n"
	"	syncw           # Make sure the unlock goes out\n"
	: "=m" (rw->lock)
	: "m" (rw->lock)
	: "memory");
#else
	__asm__ __volatile__(
	"	sync			# __raw_write_unlock	\n"
	"	sw	$0, %0					\n"
	: "=m" (rw->lock)
	: "m" (rw->lock)
	: "memory");
#endif
}

#define __raw_read_trylock(lock) generic__raw_read_trylock(lock)

static inline int __raw_write_trylock(raw_rwlock_t *rw)
{
	unsigned int tmp;
	int ret;

	if (R10000_LLSC_WAR) {
		__asm__ __volatile__(
		"	.set	noreorder	# __raw_write_trylock	\n"
		"	li	%2, 0					\n"
		"1:	ll	%1, %3					\n"
		"	bnez	%1, 2f					\n"
		"	 lui	%1, 0x8000				\n"
		"	sc	%1, %0					\n"
		"	beqzl	%1, 1b					\n"
		"	 sync						\n"
		"	li	%2, 1					\n"
		"	.set	reorder					\n"
		"2:							\n"
		: "=m" (rw->lock), "=&r" (tmp), "=&r" (ret)
		: "m" (rw->lock)
		: "memory");
	} else {
#if defined(CONFIG_CPU_CAVIUM_OCTEON) && defined(__OCTEON__)
        /* When using the Octeon compiler, use syncw instead of sync since
            it is faster */
		__asm__ __volatile__(
		"	.set	noreorder	# _raw_write_trylock	\n"
		"	syncw				# Flush the write buffer while we're waiting\n"
		"	li	%2, 0					\n"
		"1:	ll	%1, %3					\n"
		"	bnez	%1, 2f					\n"
		"	lui	%1, 0x8000				\n"
		"	sc	%1, %0			# This will go out after the syncw completes\n"
		"	beqz	%1, 1b					\n"
		"	 nop # No syncw is needed here since the sc goes out immediately\n"
		"	li	%2, 1					\n"
		"	.set	reorder					\n"
		"2:							\n"
		: "=m" (rw->lock), "=&r" (tmp), "=&r" (ret)
		: "m" (rw->lock)
		: "memory");
#else
		__asm__ __volatile__(
		"	.set	noreorder	# __raw_write_trylock	\n"
		"	li	%2, 0					\n"
		"1:	ll	%1, %3					\n"
		"	bnez	%1, 2f					\n"
		"	lui	%1, 0x8000				\n"
		"	sc	%1, %0					\n"
		"	beqz	%1, 1b					\n"
		"	 sync						\n"
		"	li	%2, 1					\n"
		"	.set	reorder					\n"
		"2:							\n"
		: "=m" (rw->lock), "=&r" (tmp), "=&r" (ret)
		: "m" (rw->lock)
		: "memory");
#endif
	}

	return ret;
}

#endif /* _ASM_SPINLOCK_H */
