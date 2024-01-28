#ifndef WAN_H
#define WAN_H

/*** Includes *********************************************************/
#include "common.h"

/*** Prototypes *******************************************************/
/*** function
 * name: wam
 * params:
 *        - OUT matrix: TODO explain matrix here
 *        - IN data: A list of all the datapoints.
 * return: SUCCESS or FAILURE.
 */
ret_codes_t wam(double *** output_matrix, DatapointList_t data);

#endif  /* WAN_H */
