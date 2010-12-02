/* klc_output.h
 * Helpers for data output.
 * This provides additional abstraction that allows to output data from 
 * the payload module without directly using printk, trace-related stuff
 * or whatever. The way the data is output is subject to change, this 
 * abstraction helps make these changes local.
 */

#ifndef KLC_OUTPUT_H_1810_INCLUDED
#define KLC_OUTPUT_H_1810_INCLUDED

#include <linux/module.h>

#include "memblock_info.h"

/* Initializes output subsystem (creates files in debugfs if necessary,
 * etc.) 
 * Returns 0 on success, negative error code on failure.
 * This function should usually be called from the module's initialization
 * function.
 */
int 
klc_output_init(void);

/* Clears the output data. For example, it may clear the contents of the 
 * files that stored information for the previous analysis session for 
 * the target module.
 *
 * This function should usually be called from on_target_load() handler
 * or the like to clear old data.
 */
void
klc_output_clear(void);

/* Finalizes output subsystem (removes files if klc_output_init() created 
 * some, etc.).
 * This function should usually be called from the module's cleanup
 * function.
 */
void
klc_output_fini(void);

/* Types of information that can be output.
 * The point is, different types of information can be output to different
 * places or distinguished in some other way.
 */
enum klc_output_type {
    KLC_UNALLOCATED_FREE,
    KLC_UNFREED_ALLOC,
    KLC_OTHER /* parameters of the target: name, code areas, etc. */
};

/* Outputs a string pointed to by 's' taking type of this information into.
 * account ('output_type').
 * The implementation defines where the string will be output and how 
 * different kinds of information will be distinguished.
 * This function is a basic block for the functions that output particular
 * data structures.
 *
 * A newline will be added at the end automatically.
 */
void
klc_print_string(enum klc_output_type output_type, const char *s);

/* Outputs first 'num_entries' elements of 'stack_entries' array as a stack
 * trace. 
 * 
 * This function cannot be used in atomic context.
 */
void
klc_print_stack_trace(enum klc_output_type output_type, 
    unsigned long *stack_entries, unsigned int num_entries);

/* Output information about the target module.
 *
 * This function cannot be used in atomic context.
 */
void
klc_print_target_module_info(struct module *target_module);

/* Helpers to output kedr_memblock_info structures corresponding to 
 * suspicious memory allocation and deallocation events.
 *
 * Cannot be used in atomic context.
 */
void 
klc_print_alloc_info(struct kedr_memblock_info *alloc_info);

void 
klc_print_dealloc_info(struct kedr_memblock_info *dealloc_info);

#endif /* KLC_OUTPUT_H_1810_INCLUDED */
