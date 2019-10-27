/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : main
 * @created     : Sunday Oct 27, 2019 11:32:05 MDT
 */

#include <curl/curl.h>
#include <stdio.h>

int
main(void)
{
  CURL* curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_ALL);

  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/tagpoint");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=theo&project=test");

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      fprintf(
        stderr, "curl_easy_perform failed: %s\n", curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}
