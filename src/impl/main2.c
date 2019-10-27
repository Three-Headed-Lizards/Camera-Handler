/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : main2
 * @created     : Sunday Oct 27, 2019 15:44:37 MDT
 */

#include "posthandler.h"
#include <unistd.h>

int
main()
{
  tagpoint tag;

  if (create_tagpoint(&tag, "Theo")) {
    return errno;
  }

  tagpoint_setopt(&tag, MILLISECONDS, NULL);

  // Sees a tag
  update_tag_point_timer(&tag);

  sleep(3); // seeing the tag

  char dest[90];
  tag_point_to_post_request(&tag, dest);

  printf("%s\n", dest);

  return 1;
}
