// Three-state boolean logic library

// Copyright Douglas Gregor 2002-2004. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Modifications by Orxonox to shape the third state into 'dontcare' instead
// of 'indeterminate'. The difference is that 'dontcare' is actually a value
// so that (dontcare == dontcare).
// Also removed all logic operators except for == and !=


// For more information, see http://www.boost.org
#ifndef ORXONOX_TRIBOOL_H
#define ORXONOX_TRIBOOL_H

namespace orxonox {

/**
 * INTERNAL ONLY
 * The type of the 'dontcare' keyword.
 */
struct dontcare_keyword_t { };

/**
 * \brief Keyword for the dontcare tribool value
 */
const dontcare_keyword_t dontcare = dontcare_keyword_t();

/**
 * \brief A 3-state boolean type.
 *
 * 3-state boolean values are either true, false, or
 * dontcare.
 */
class tribool
{
public:
  /**
   * Construct a new 3-state boolean value with the value 'false'.
   *
   * \throws nothrow
   */
  tribool() : value(false_value) {}

  /**
   * Construct a new 3-state boolean value with the given boolean
   * value, which may be \c true or \c false.
   *
   * \throws nothrow
   */
  tribool(bool value) : value(value? true_value : false_value) {}

  /**
   * Construct a new 3-state boolean value with an dontcare value.
   *
   * \throws nothrow
   */
  tribool(dontcare_keyword_t) : value(dontcare_value) {}

  /**
   * \brief Compare tribools for equality
   *
   * \returns the result of comparing two tribool values.
   * \throws nothrow
   */
  inline bool operator==(tribool y)
  {
    return (this->value == y.value);
  }

  /**
   * \overload
   */
  inline bool operator==(bool y) { return (*this) == tribool(y); }

  /**
   * \overload
   */
  inline bool operator==(dontcare_keyword_t)
  { return tribool(dontcare) == (*this); }

  /**
   * \brief Compare tribools for inequality
   *
   * \returns the result of comparing two tribool values for inequality.
   * \throws nothrow
   */
  inline bool operator!=(tribool y)
  {
    return !((*this) == y);
  }

  /**
   * \overload
   */
  inline bool operator!=(bool y) { return (*this) != tribool(y); }

  /**
   * \overload
   */
  inline bool operator!=(dontcare_keyword_t)
  { return (*this) != tribool(dontcare); }

  /**
   * The actual stored value in this 3-state boolean, which may be false, true,
   * or dontcare.
   */
  enum value_t { false_value, true_value, dontcare_value } value;
};

/**
 * \overload
 */
inline bool operator==(bool x, tribool y) { return tribool(x) == y; }

/**
 * \overload
 */
inline bool operator==(dontcare_keyword_t, tribool x)
{ return tribool(dontcare) == x; }

/**
 * \overload
 */
inline bool operator!=(bool x, tribool y) { return tribool(x) != y; }

/**
 * \overload
 */
inline bool operator!=(dontcare_keyword_t, tribool x)
{ return tribool(dontcare) != x; }

} // end namespace orxonox

#endif // ORXONOX_TRIBOOL_H

