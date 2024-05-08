/* -*- mode: c++ -*-
 *
 * lag/utils/URI.h --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Nicolas Roussel
 *
 *
 */

#ifndef lag_utils_URI_h
#define lag_utils_URI_h

/*

  Based on:
  http://www.ics.uci.edu/pub/ietf/uri/rfc2396.txt

  FIXME: Should be updated to:
  http://labs.apache.org/webarch/uri/rfc/rfc3986.html

  See also:
  http://www.ics.uci.edu/~fielding/
  http://www.ics.uci.edu/~fielding/url/

*/

#include <string>
#include <sstream>

namespace lag {
  
  struct URI {

    /**
     * @brief Scheme is used to represent the type of the device or transfer function.
     */
    std::string scheme ;

    /**
     * @brief An opaque URI is an absolute URI whose scheme-specific part does not begin with a slash character ('/')
     */
    std::string opaque ;

    std::string user, password ;
    std::string host ;
    int port ;
    std::string path ;

    /**
     * @brief The part of URI after ? which sets the parameters of the scheme
     */
    std::string query ;

    /**
     * @brief The part of the URI after #.
     */
    std::string fragment ;
 
  public:

    typedef enum {
      NONE = 0,
      NORMAL = 1,
      UNSAFE = 2,
      RESERVED = 4
    } URIENCODING ;

    static std::string encode(const std::string &src, int flags = NORMAL) ;
    static std::string decode(const std::string &src) ;

    /**
     * @brief Writes the value of the key variable into the value variable of the given URI string.
     * @param q Given URI string.
     * @param key Parameter, the value of which is queried.
     * @param value The value of the queried parameter.
     * @return Is the value of the key parameter found.
     */
    //@{
    static bool getQueryArg(const std::string &q, const std::string &key, std::string *value=0) ;
    static bool getQueryArg(const std::string &q, const std::string &key, bool *value) ;
    static bool getQueryArg(const std::string &q, const std::string &key, int *value) ;
    static bool getQueryArg(const std::string &q, const std::string &key, unsigned int *value) ;
    static bool getQueryArg(const std::string &q, const std::string &key, unsigned long *value) ;
    static bool getQueryArg(const std::string &q, const std::string &key, double *value) ;
    static bool getQueryArg(const std::string &q, const std::string &key, float *value) ;
    //@}


    /**
     * @brief Appends a new parameter to the given URI string
     * @param q URI string.
     * @param key A new parameter to append.
     * @param value The value of the given parameter if any.
     * @return Modified URI string.
     */
    //@{
    static std::string &addQueryArg(std::string &q, const std::string &key) ;
    static std::string &addQueryArg(std::string &q, const std::string &key, std::string value) ;
    static std::string &addQueryArg(std::string &q, const std::string &key, double value) ;
    static std::string &addQueryArg(std::string &q, const std::string &key, int value) ;
    //@}

    /**
     * @brief Removes a parameter from the given URI string
     * @param q URI string
     * @param key Parameter to remove
     * @return Modified URI string
     */
    static std::string &removeQueryArg(std::string &q, const std::string &key) ;

    // --------------------------------------------------------------------------

    URI(void) : port(0) {}
    URI(const URI& src) ;
    URI(const std::string& s) { load(s) ; }
    URI(const char *s) { if (s) load(s) ; }

    URI& operator = (const URI& src) ;

    bool operator ==(const URI &other) const ;
    bool operator !=(const URI &other) const ;

    /**
     * @brief Compares scheme, opaque, host, port and path
     * @param URI instance to be compared with.
     * @return The boolean result of comparison.
     */
    bool resemble(const URI &other) const ;

    void clear(void) ;
    void load(const std::string &uri) ;

    bool isEmpty(void) const ;

    void generalize(void) ;

    std::string asString(void) const ;

    void debug(std::ostream& out) const ;

  } ;
  
}

#endif
