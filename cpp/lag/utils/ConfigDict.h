/* -*- mode: c++ -*-
 *
 * lag/utils/ConfigDict.h --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Nicolas Roussel
 *
 *
 */

#ifndef lag_utils_ConfigDict_h
#define lag_utils_ConfigDict_h

#include <string>
#include <map>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace lag {

  class ConfigDict {

  private:

    typedef std::map<std::string, std::string> ConfigDictImpl ;

    ConfigDictImpl dict ;

  protected:

    template <class X> X str2X(const std::string &str) const ;
    bool str2bool(const std::string &str) const ;

  public:

    static const std::string defkvpairseps ;
    static const std::string defkvseps ;
    static const std::string defcomment ;
    static const std::string defkvpairsep ;
    static const std::string defkvsep ;

    typedef ConfigDictImpl::const_iterator const_iterator ;
    typedef ConfigDictImpl::iterator iterator ;

    ConfigDict(void) {}

    ConfigDict(std::string data, 
	       std::string kvpairseps=defkvpairseps, std::string kvseps=defkvseps, 
	       std::string commentmarker=defcomment) {
      loadFrom(data, kvpairseps, kvseps, commentmarker) ;
    }
   
    void clear(void) { dict.clear() ; }

    size_t size(void) const { return dict.size() ; }

    iterator begin(void) { return dict.begin() ; }
    iterator end(void) { return dict.end() ; }

    const_iterator begin(void) const { return dict.begin() ; }
    const_iterator end(void) const { return dict.end() ; }

    bool load(std::string data, 
	      std::string kvpairseps=defkvpairseps, std::string kvseps=defkvseps,
	      std::string commentmarker=defcomment) ;

    bool loadFrom(std::string filename, 
		  std::string kvpairseps=defkvpairseps, std::string kvseps=defkvseps,
		  std::string commentmarker=defcomment) ;

    void updateFrom(const ConfigDict &other) ;
    
    template <class X> void set(const std::string &key, X val) ;

    template <class X> X setdefault(const std::string &key, const X defval) ;
    std::string setdefault(const std::string &key, const char *defval) ;
    
    bool get(const char *key) const ;
    template <class X> X get(const std::string &key) const ;
    template <class X> X get(const char *key) const ;

    std::string dump(std::string kvpairsep=defkvpairsep, std::string kvsep=defkvsep) const ;

    void saveAs(std::string filename, std::string kvpairsep=defkvpairsep, std::string kvsep=defkvsep) ;
    void exportToJSON(std::string filename) const ;

  } ;

  // --------------------------------------------------------------

  template <class X> X ConfigDict::str2X(const std::string &str) const {
    std::stringstream tmp ;
    tmp << str ;
    X result ;
    tmp >> result ;
    return result ;
  }

  // --------------------------------------------------------------

  template <> inline void ConfigDict::set(const std::string &key, std::string value) {
    dict[key] = value ;
  }

  template <class X> inline void ConfigDict::set(const std::string &key, X val) {
    std::stringstream tmp ;
    tmp << val ;
    dict[key] = tmp.str() ;
  }

  // --------------------------------------------------------------

  template <> inline bool ConfigDict::setdefault(const std::string &key, const bool defval) {
    iterator i = dict.find(key) ;
    if (i!=dict.end()) return str2bool(i->second) ;
    dict[key] = defval?"true":"false" ;
    return defval ;
  }

  template <> inline std::string ConfigDict::setdefault(const std::string &key, const std::string defval) {
    iterator i = dict.find(key) ;
    if (i!=dict.end()) return i->second ;
    dict[key] = defval ;
    return defval ;
  }

  template <class X> inline X ConfigDict::setdefault(const std::string &key, const X defval) {
    iterator i = dict.find(key) ;
    if (i!=dict.end()) return str2X<X>(i->second) ;
    std::stringstream tmp ;
    tmp << defval ;
    tmp >> dict[key] ;
    return defval ;
  }

  // --------------------------------------------------------------

  template <class X> X ConfigDict::get(const std::string &key) const {
    const_iterator i = dict.find(key) ;
    if (i==dict.end()) return X() ;
    return str2X<X>((*i).second) ;
  }

  template <class X> X ConfigDict::get(const char *key) const {
    return get<X>(std::string(key)) ;
  }

}

#endif
