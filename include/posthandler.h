/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : posthandler
 * @created     : Sunday Oct 27, 2019 11:42:06 MDT
 */

#ifndef POSTHANDLER_H

#define POSTHANDLER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TIME_HEADER_SIZE 30
#define MAX_USERNAME_LENGTH 30
#define POST_MAX_LENGTH 200

// Tag options - orred to get a value
#define TAGPOINT_OPT_T uint16_t
#define USERNAME 1
#define TAGPOINT_POINT (1 << 1)
#define TAGPOINT_LOST (1 << 2)
#define TIME_UNIT_TYPE 4

#define TAGPOINT_T uint8_t
#define TAGPOINT_DUR_T uint64_t
#define USER_ID_T uint32_t

// OPTIONS
#define TAG_POINT 1

#define MICROSECONDS (1 << 3)
#define MILLISECONDS (1 << 4)
#define SECONDS (1 << 5)
#define DECISECONDS (1 << 6)
#define MINUTES (1 << 7)
#define TIME_MASK                                                              \
  ~(MICROSECONDS | MILLISECONDS | SECONDS | DECISECONDS | MINUTES)

  /**
   * @brief Tag point is the handle for post requests, ie
   * it contains data about a user and tag duration
   */
  typedef struct
  {
    char user_name[MAX_USERNAME_LENGTH];
    size_t user_name_length;

    char timestamp[TIME_HEADER_SIZE];
    size_t timestamp_length;

    USER_ID_T userid;

    TAGPOINT_OPT_T tag_options;

    struct timespec index;
    TAGPOINT_DUR_T dur;
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
  int tagpoint_setopt(tagpoint* point, TAGPOINT_OPT_T, const void* data);

  /**
   * @brief Serializes tagpoint into a buffer for transport onto a post request
   *
   * @param point The tagpoint to serialize
   * @param destination The buffer that is
   *
   * @return
   */
  int tag_point_to_post_request(tagpoint* point, char* destination);

  int incriment_tag_point_timer(tagpoint* point);

  int update_tag_point_timer(tagpoint* point);

  int update(tagpoint* point, int tag_seen);

#ifdef __cplusplus
} // end extern "C"
#endif

#endif /* end of include guard POSTHANDLER_H */
