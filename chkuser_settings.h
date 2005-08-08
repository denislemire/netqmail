
/*
 *
 * 'chkuser_settings.h' v.2.0.8
 * for qmail/netqmail > 1.0.3 and vpopmail > 5.3.x
 *
 * Author: Antonio Nati tonix@interazioni.it
 * All rights on this software and
 * the identifying words chkusr and chkuser kept by the author
 *
 * This software may be freely used, modified and distributed,
 * but this lines must be kept in every original or derived version.
 * Original author "Antonio Nati" and the web URL
 * "http://www.interazioni.it/opensource"
 * must be indicated in every related work or web page
 *
 */

/*
 * the following line enables debugging of chkuser
 */
/* #define CHKUSER_DEBUG */

/*
 * The following line moves DEBUG output from STDOUT (default) to STDERR
 * Example of usage within sh: ./qmail-smtpd 2> /var/log/smtpd-debug.log
 */
/* #define CHKUSER_DEBUG_STDERR */

/*
 * Uncomment the following define if you want chkuser ALWAYS enabled.
 * If uncommented, it will check for rcpt existance despite any .qmail-default
 * setting.
 * So, unsomments this if you are aware that ALL rcpt in all domains will be
 * ALWAYS checked.
 */
/* #define CHKUSER_ALWAYS_ON */

/*
 * The following defines which virtual manager is used.
 * Up to know, only vpopmail, but versions with pure qmail are in the mind.
 */
#define CHKUSER_VPOPMAIL

/*
 * Uncomment the following line if you want chkuser to work depending on a VARIABLE setting
 * VALUE HERE DEFINED is the name of the variable
 * Values admitted inside the variable: NONE | ALWAYS | DOMAIN
 * 		NONE 	= chkuser will not work
 *		ALWAYS	= chkuser will work always
 *		DOMAIN	= chkuser will work depending by single domain settings
 * if CHKUSER_ALWAYS_ON is defined, this define is useless
 * if CHKUSER_STARTING_VARIABLE is defined, and no variable or no value is set, then chkuser is disabled
 */
/* #define CHKUSER_STARTING_VARIABLE "CHKUSER_START" */

/*
 * Uncomment this to enable uid/gid changing
 * (switching UID/GID is NOT compatible with TLS; you may keep this commented if you have TLS)
 */
/* #define CHKUSER_ENABLE_UIDGID */

/*
 * Uncomment this to check if a domain is ALWAYS specified in rcpt addresses
 */
#define CHKUSER_DOMAIN_WANTED

/*
 * Uncomment this to check for vpopmail users
 */
#define CHKUSER_ENABLE_USERS

/*
 * Uncomment this to check for alias
 */
#define CHKUSER_ENABLE_ALIAS

/*
 * The following #define set the character used for lists extensions
 * be careful: this is a  single char '-' definition, not a "string"
 */
#define CHKUSER_EZMLM_DASH '-'

/*
 * Uncomment this to set an alternative way to check for bouncing enabling;
 * with this option enabled, the file here defined 
 * will be searched, inside the domain dir, in order to check if bouncing is enabled
 * The content of this file is not important, just it's existence is enough
 */
/* #define CHKUSER_SPECIFIC_BOUNCING ".qmailchkuser-bouncing" */

/*
 * This is the string to look for inside .qmail-default
 * Be careful, chkuser looks within the first 1023 characters of .qmail-default for
 * this string (despite the line containing the string is working or commented).
 */
#define CHKUSER_BOUNCE_STRING "bounce-no-mailbox"

/*
 * This is to enable auth open checking
 * it is useful to avoid bouncing if MySQL/LDAP/PostGRES/etc are down or not reachable
 */
/* #define CHKUSER_ENABLE_VAUTH_OPEN */

/*
 * Uncomment to enable logging of rejected recipients and variuos limits reached
 */
/* #define CHKUSER_ENABLE_LOGGING */

/*
 * Uncomment to enable logging of "good" rcpts
 * valid only if CHKUSER_ENABLE_LOGGING is defined
 */
/* #define CHKUSER_LOG_VALID_RCPT */

/*
 * Uncomment to enable usage of a variable escluding any check on the sender.
 * The variable should be set in tcp.smtp for clients, with static IP, whose mailer
 * is composing bad sender addresses
 */
/* #define CHKUSER_SENDER_NOCHECK_VARIABLE "SENDER_NOCHECK" */

/*
 * Uncomment to enable usage of "#" and "+" characters within sender address
 * This is used by SRS (Sender Rewriting Scheme) products
 */
/* #define CHKUSER_ALLOW_SENDER_SRS */

/*
 * If you need more additional characters to be accepted within sender address
 * uncomment one of the following #define and edit the character value.
 * Be careful to use '*' (single hiphen) and NOT "*" (double hiphen) around the
 * wanted char.
 */
/* #define CHKUSER_ALLOW_SENDER_CHAR_1 '$' */
/* #define CHKUSER_ALLOW_SENDER_CHAR_2 '%' */
/* #define CHKUSER_ALLOW_SENDER_CHAR_3 '£' */
/* #define CHKUSER_ALLOW_SENDER_CHAR_4 '?' */
/* #define CHKUSER_ALLOW_SENDER_CHAR_5 '*' */

/*
 * The following #define sets the minimum length of a domain:
 * as far as I know, "k.st" is the shortest domain, so 4 characters is the
 * minimum length.
 * This value is used to check formally a domain name validity.
 * if CHKUSER_SENDER_FORMAT is undefined, no check on length is done.
 * If you comment this define, no check on length is done.
 */
#define CHKUSER_MIN_DOMAIN_LEN 4

/*
 * Uncomment to enable logging of "good" senders
 * valid only if CHKUSER_ENABLE_LOGGING is defined
 */
#define CHKUSER_LOG_VALID_SENDER

/*
 * Uncomment to define a variable which contains the max recipients number
 * this will return always error if total recipients exceed this limit.
 * The first reached, between CHKUSER_RCPT_LIMIT_VARIABLE and CHKUSER_WRONGRCPT_LIMIT_VARIABLE,
 * makes chkuser rejecting everything else
 */
#define CHKUSER_RCPT_LIMIT_VARIABLE "CHKUSER_RCPTLIMIT"

/*
 * Uncomment to define a variable which contains the max unknown recipients number
 * this will return always error if not existing recipients exceed this limit.
 * The first reached, between CHKUSER_RCPT_LIMIT_VARIABLE and CHKUSER_WRONGRCPT_LIMIT_VARIABLE,
 * makes chkuser rejecting everything else
 */
#define CHKUSER_WRONGRCPT_LIMIT_VARIABLE "CHKUSER_WRONGRCPTLIMIT"

/*
 * Uncomment to define the variable containing the percent to check for.
 * Remember to define externally (i.e. in tcp.smtp) the environment variable containing
 * the limit percent.
 * If the variable is not defined, or it is <= 0, quota checking is not performed.
 */
#define CHKUSER_MBXQUOTA_VARIABLE "CHKUSER_MBXQUOTA"

/*
 * Delay to wait for each not existing recipient
 * value is expressed in milliseconds
 */
#define CHKUSER_ERROR_DELAY 1000

/*
 * Uncomment to consider rcpt errors on address format and MX as intrusive
 *
 */
#define CHKUSER_RCPT_DELAY_ANYERROR

/*
 * Uncomment to consider sender errors on address format and MX as intrusive
 *
 */
#define CHKUSER_SENDER_DELAY_ANYERROR

#define CHKUSER_NORCPT_STRING "511 sorry, no mailbox here by that name (#5.1.1 - chkuser)\r\n"
#define CHKUSER_RESOURCE_STRING "430 system temporary unavailable, try again later (#4.3.0 - chkuser)\r\n"
#define CHKUSER_MBXFULL_STRING "522 sorry, recipient mailbox is full (#5.2.2 - chkuser)\r\n"
#define CHKUSER_MAXRCPT_STRING "571 sorry, reached maximum number of recipients for one session (#5.7.1 - chkuser)\r\n"
#define CHKUSER_MAXWRONGRCPT_STRING "571 sorry, you are violating our security policies (#5.1.1 - chkuser)\r\n"
#define CHKUSER_DOMAINMISSING_STRING "511 sorry, you must specify a domain (#5.1.1 - chkuser)\r\n"
#define CHKUSER_RCPTFORMAT_STRING "511 sorry, recipient address has invalid format (#5.1.1 - chkuser)\r\n"
#define CHKUSER_RCPTMX_STRING "511 sorry, can't find a valid MX for rcpt domain (#5.1.1 - chkuser)\r\n"
#define CHKUSER_SENDERFORMAT_STRING "571 sorry, sender address has invalid format (#5.7.1 - chkuser)\r\n"
#define CHKUSER_SENDERMX_STRING "511 sorry, can't find a valid MX for sender domain (#5.1.1 - chkuser)\r\n"
#define CHKUSER_INTRUSIONTHRESHOLD_STRING "571 sorry, you are violating our security policies (#5.7.1 - chkuser)\r\n"
#define CHKUSER_NORELAY_STRING "553 sorry, that domain isn't in my list of allowed rcpthosts (#5.5.3 - chkuser)\r\n"

/***************************************************
 *
 *      new/modified defines in/from 2.0.6
 *
 **************************************************/

/*
 * Before version 5.3.25, vpopmail used the function vget_real_domain()
 * to get the real name of a domain (useful if rcpt domain is aliasing
 * another domain).
 * From version 5.3.25, this call is not available and has been
 * substituted by other calls.
 *
 *        must be enabled if vpopmail version< 5.3.5
 *        must be disabled  if vpopmail version => 5.3.5 *
 */
/* #define CHKUSER_ENABLE_VGET_REAL_DOMAIN */

/***************************************************
 *
 *      new/modified defines in/from 2.0.7
 *
 **************************************************/

/*
 * Uncomment next define to accept recipients for
 * aliases that have a -default extension
 */
/* #define CHKUSER_ENABLE_ALIAS_DEFAULT */


/*
 * Uncomment to enable usage of "#" and "+" characters within rcpt address
 * This is used by SRS (Sender Rewriting Scheme) products
 */
/* #define CHKUSER_ALLOW_RCPT_SRS */

/*
 * If you need more additional characters to be accepted within rcpt address
 * uncomment one of the following #define and edit the character value.
 * Be careful to use '*' (single hiphen) and NOT "*" (double hiphen) around the
 * wanted char.
 */
/* #define CHKUSER_ALLOW_RCPT_CHAR_1 '$' */
/* #define CHKUSER_ALLOW_RCPT_CHAR_2 '%' */
/* #define CHKUSER_ALLOW_RCPT_CHAR_3 '£' */
/* #define CHKUSER_ALLOW_RCPT_CHAR_4 '?' */
/* #define CHKUSER_ALLOW_RCPT_CHAR_5 '*' */

/*
 * This define has been eliminated.
 * Turning it ON or OFF has no effect, as we consider the existence
 * of #define VALIAS inside ~vpopmail/include/vpopmail_config.h
 */
/* #define CHKUSER_ENABLE_VALIAS */

/*
 * Uncomment this to enable user extension on names (i.e. TMDA)
 * (for mailing lists this is done without checking this define)
 * This define substitutes #define CHKUSER_ENABLE_EXTENSIONS
 */
/* #define CHKUSER_ENABLE_USERS_EXTENSIONS */

/*
 * Enables checking for EZMLM lists
 * this define substitutes #define CHKUSER_ENABLE_LISTS
 *
 */
#define CHKUSER_ENABLE_EZMLM_LISTS

/*
 * Help identifying remote authorized IPs giving them a descriptive name
 * Can be put in tcp.smtp, and will be displayed inside chkuser log
 * Substitutes RELAYCLIENT in chkuser logging
 */
#define CHKUSER_IDENTIFY_REMOTE_VARIABLE "CHKUSER_IDENTIFY"

/*
 * The following #define set the character used for users extensions
 * be careful: this is a  single char '-' definition, not a "string"
 * this define substitutes #define CHKUSER_EXTENSION_DASH
 * MUST be defined if CHKUSER_ENABLE_USERS_EXTENSIONS is defined
 */
#define CHKUSER_USERS_DASH '-'

/*
 * New error strings for SOFT DNS problems
 */
#define CHKUSER_RCPTMX_TMP_STRING "451 DNS temporary failure (#4.5.1 - chkuser)\r\n"
#define CHKUSER_SENDERMX_TMP_STRING "451 DNS temporary failure (#4.5.1 - chkuser)\r\n"

/*
 * Enables checking for mailman lists
 *
 */
/* #define CHKUSER_ENABLE_MAILMAN_LISTS */

/*
 * Identifies the pattern string to be searched within mailman aliases
 *
 */
#define CHKUSER_MAILMAN_STRING "mailman"

/*
 * The following #define set the character used for mailman lists extensions
 * be careful: this is a  single char '-' definition, not a "string"
 */
#define CHKUSER_MAILMAN_DASH '-'


/*
 * Enables final clean-up routine of chkuser
 * This routine cleans open DB connections used for checking users and valiases
 */
#define CHKUSER_DB_CLEANUP

/***************************************************
 *
 *      new/modified defines in/from 2.0.8
 *
 **************************************************/

/*
 * The following defines are NO MORE used. NULL SENDER rejecting breaks RFC
 * compatibility, and makes harder to handle e-mail receipts.
 * Please comment or delete them from your chkuser_settings.h.
 */
/* #define CHKUSER_ACCEPT_NULL_SENDER */
/* #define CHKUSER_ENABLE_NULL_SENDER_WITH_TCPREMOTEHOST */

/*
 * Uncomment to enable checking of user and domain format for rcpt addresses
 *      user    =       [a-z0-9_-]
 *      domain  =       [a-z0-9-.] with not consecutive "-.", not leading or ending "-."
 */
/* #define CHKUSER_RCPT_FORMAT */

/*
 * Uncomment to enable checking of domain MX for rcpt addresses
 * It works on any rcpt address domain that is not inside rcpthosts
 */
/* #define CHKUSER_RCPT_MX */

/*
 * Uncomment to enable checking of user and domain format for sender address
 *      user    =       [a-z0-9_-]
 *      domain  =       [a-z0-9-.] with not consecutive "-.", not leading or ending "-."
 */
/* #define CHKUSER_SENDER_FORMAT */

/*
 * Uncomment to enable checking of domain MX for sender address
 * it works on the first rcpt address, despite of any domain setting on chkuser
 */
/* #define CHKUSER_SENDER_MX */

/*
 * Delay to add, for each not existing recipient, to the initial CHKUSER_ERROR_DELAY value
 * value is expressed in milliseconds
 */
#define CHKUSER_ERROR_DELAY_INCREASE 300

