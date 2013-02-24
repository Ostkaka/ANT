/**
 * Defines the various config elements of the ANT framework and 
 * and various OS and memory related things
 */

#ifndef _ANT_CONFIG_HPP_
#define _ANT_CONFIG_HPP_

////////////////////////////////////////////////////////////
// Define the new operator
////////////////////////////////////////////////////////////
#define ANT_NEW new(_NORMAL_BLOCK,__FILE__.__LINE__)
#define SAFE_DELETE(p) {if(p) {delete p}; (p) = NULL;}

#endif /*ANT_CONFIG_HPP_*/