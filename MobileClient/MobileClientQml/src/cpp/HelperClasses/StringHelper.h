#ifndef STRING_HELPER_H
#define STRING_HELPER_H

class QString;

//-----------------------------------------------------------------------------------------------------------------------------

/*!
 * \brief The StringHelper class provides general string related helper methods.
 */
class StringHelper
{
public:

   /*!
    * \brief Quotes the given string.
    * \param string The string to quote.
    * \return The quoted string.
    */
   static QString quoted (const QString &string);

   /*!
    * \brief If quotes are present they will be doubled.
    * \param string The string to double quote.
    * \return The double quoted string.
    */
   static QString doubleQuote(const QString &string);

   /*!
    * \brief Quote the given string for SQL.
    * \param string The string to quote.
    * \return The string quoted in order to avoid SQL injection.
    */
   static QString sqlQuote(const QString &string);
};

#endif
