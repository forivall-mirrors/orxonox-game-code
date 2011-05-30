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

// Forward declare tribool
class tribool;

/// INTERNAL ONLY
namespace detail {
/**
 * INTERNAL ONLY
 *
 * \brief A type used only to uniquely identify the 'dontcare'
 * function/keyword.
 */
struct dontcare_t
{
};

} // end namespace detail

/**
 * INTERNAL ONLY
 * The type of the 'dontcare' keyword. This has the same type as the
 * function 'dontcare' so that we can recognize when the keyword is
 * used.
 */
typedef bool (*dontcare_keyword_t)(tribool, detail::dontcare_t);

/**
 * \brief Keyword and test function for the dontcare tribool value
 *
 * The \c dontcare function has a dual role. It's first role is
 * as a unary function that tells whether the tribool value is in the
 * "dontcare" state. It's second role is as a keyword
 * representing the dontcare (just like "true" and "false"
 * represent the true and false states).
 *
 * \returns <tt>x.value == tribool::dontcare_value</tt>
 * \throws nothrow
 */
inline bool
dontcare(tribool x, detail::dontcare_t dummy = detail::dontcare_t());

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
   * The actual stored value in this 3-state boolean, which may be false, true,
   * or dontcare.
   */
  enum value_t { false_value, true_value, dontcare_value } value;
};

// Check if the given tribool has an dontcare value. Also doubles as a
// keyword for the 'dontcare' value
inline bool dontcare(tribool x, detail::dontcare_t)
{
  return x.value == tribool::dontcare_value;
}

/**
 * \brief Compare tribools for equality
 *
 * \returns the result of comparing two tribool values, according to
 * the following table:
 *       <table border=1>
 *          <tr>
 *            <th><center><code>==</code></center></th>
 *            <th><center>false</center></th>
 *            <th><center>true</center></th>
 *            <th><center>false</center></th>
 *          </tr>
 *          <tr>
 *            <th><center>false</center></th>
 *            <td><center>true</center></td>
 *            <td><center>false</center></td>
 *            <td><center>false</center></td>
 *          </tr>
 *          <tr>
 *            <th><center>true</center></th>
 *            <td><center>false</center></td>
 *            <td><center>true</center></td>
 *            <td><center>false</center></td>
 *          </tr>
 *          <tr>
 *            <th><center>dontcare</center></th>
 *            <td><center>false</center></td>
 *            <td><center>false</center></td>
 *            <td><center>false</center></td>
 *          </tr>
 *      </table>
 * \throws nothrow
 */
inline bool operator==(tribool x, tribool y)
{
  return (x.value == y.value);
}

/**
 * \overload
 */
inline bool operator==(tribool x, bool y) { return x == tribool(y); }

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
inline bool operator==(tribool x, dontcare_keyword_t)
{ return tribool(dontcare) == x; }

/**
 * \brief Compare tribools for inequality
 *
 * \returns the result of comparing two tribool values for inequality,
 * according to the following table:
 *       <table border=1>
 *           <tr>
 *             <th><center><code>!=</code></center></th>
 *             <th><center>false</center></th>
 *             <th><center>true</center></th>
 *             <th><center>dontcare</center></th>
 *           </tr>
 *           <tr>
 *             <th><center>false</center></th>
 *             <td><center>false</center></td>
 *             <td><center>true</center></td>
 *             <td><center>true</center></td>
 *           </tr>
 *           <tr>
 *             <th><center>true</center></th>
 *             <td><center>true</center></td>
 *             <td><center>false</center></td>
 *             <td><center>true</center></td>
 *           </tr>
 *           <tr>
 *             <th><center>true</center></th>
 *             <td><center>true</center></td>
 *             <td><center>true</center></td>
 *             <td><center>false</center></td>
 *           </tr>
 *       </table>
 * \throws nothrow
 */
inline bool operator!=(tribool x, tribool y)
{
  return !(x == y);
}

/**
 * \overload
 */
inline bool operator!=(tribool x, bool y) { return x != tribool(y); }

/**
 * \overload
 */
inline bool operator!=(bool x, tribool y) { return tribool(x) != y; }

/**
 * \overload
 */
inline bool operator!=(dontcare_keyword_t, tribool x)
{ return tribool(dontcare) != x; }

/**
 * \overload
 */
inline bool operator!=(tribool x, dontcare_keyword_t)
{ return x != tribool(dontcare); }

} // end namespace orxonox

#endif // ORXONOX_TRIBOOL_H

