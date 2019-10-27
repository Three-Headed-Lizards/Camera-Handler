/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : posthandler
 * @created     : Sunday Oct 27, 2019 11:42:06 MDT
 */

#ifndef POSTHANDLER_H

#define POSTHANDLER_H

#include <inttypes.h>

#define TIME_HEADER_SIZE 30

#define MAX_USERNAME_LENGTH 30

// Tag options - orred to get a value
#define TAGPOINT_OPT uint16_t
#define USERNAME (1)
#define TAGPOINT_POINT (1 << 1)

#define TAGPOINT uint16_t

typedef struct {
  char user_name [MAX_USERNAME_LENGTH]; 
  size_t user_name_length;

  char timestamp [TIME_HEADER_SIZE];  
  size_t timestamp_length;

  TAGPOINT point;
} tagpoint;

/**
 * @brief Creates and sets a tagpoint
 *
 * @param tag The tag to initialize
 * @param username The User name of the tag
 *
 * @return The status
 */
int create_tagpoint(tagpoint* tag, char* username);

/**
 * @brief Sets tagpoint options
 *
 * @param point The pointer to the tagpoint
 * @param TAGPOINT_OPT The tagpoint option, for now pretty few options
 * @param data The data to set
 *
 * @return Status of setter
 */
int tagpoint_setopt(tagpoint* point, TAGPOINT_OPT, void* data);

/**
 * @brief Serializes tagpoint into a buffer for transport onto a post request
 *
 * @param point The tagpoint to serialize
 * @param destination The buffer that is 
 *
 * @return 
 */
int tagpoint_serialize(tagpoint* point, char* destination);



#endif /* end of include guard POSTHANDLER_H */

