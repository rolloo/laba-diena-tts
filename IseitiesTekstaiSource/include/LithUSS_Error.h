///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.ra\xF0tija.lt)
// Sintezatoriaus komponentas LIEPA.exe
// Failas LithUSS_Error.h
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef LITHUSS_ERROR_H_INCLUDED
#define LITHUSS_ERROR_H_INCLUDED

#include "common.h"

EXPORT char* getLUSSErrorMessages(int err);

#define	NO_ERR 0						//No error occurred

//TextNormalization klaidu kodai
#define	ERROR_TEXTNORMALIZATION_OPENING_RULES_FILE -20
#define	ERROR_TEXTNORMALIZATION_OPENING_FILE_SPECIFIED_IN_RULES_FILE -21

#define	ERROR_TEXTNORMALIZATION_BUFFER_OVERFLOW -22

//TranscrLUSS klaidu kodai
#define	ERROR_TRANSCRLUSS_OPENING_TRANSCR_RULES_FILE -35
#define	ERROR_TRANSCRLUSS_READING_TRANSCR_RULES_FILE -36

//UnitSelection klaidu kodai
#define	ERROR_UNITSELECTION_OPENING_SETTINGS_FILE -40
#define	ERROR_UNITSELECTION_READING_SETTINGS_FILE -41
#define	ERROR_UNITSELECTION_OPENING_JOINING_COSTS_FILE -42
#define	ERROR_UNITSELECTION_READING_JOINING_COSTS_FILE -43
#define	ERROR_UNITSELECTION_OPENING_LEFT_SUBSTITUTION_COSTS_FILE -44
#define	ERROR_UNITSELECTION_READING_LEFT_SUBSTITUTION_COSTS_FILE -45
#define	ERROR_UNITSELECTION_OPENING_RIGHT_SUBSTITUTION_COSTS_FILE -46
#define	ERROR_UNITSELECTION_READING_RIGHT_SUBSTITUTION_COSTS_FILE -47
#define	ERROR_UNITSELECTION_OPENING_DB_FON_FILE -48
#define	ERROR_UNITSELECTION_DB_FON_EXCEEDED_MAX_UNITS -49
#define	ERROR_UNITSELECTION_READING_DB_FON_FILE -50
#define	ERROR_UNITSELECTION_OPENING_DB_FON_WEIGHTS_FILE -51
#define	ERROR_UNITSELECTION_READING_DB_FON_WEIGHTS_FILE -52

#define	ERROR_UNITSELECTION_MEMORY_ALLOCATION -53
#define	ERROR_UNITSELECTION_UNIT_NOT_FOUND -54
#define	ERROR_UNITSELECTION_UNEXPECTED_1 -55
#define	ERROR_UNITSELECTION_UNEXPECTED_2 -56
#define	ERROR_UNITSELECTION_UNEXPECTED_3 -57
#define	ERROR_UNITSELECTION_UNEXPECTED_4 -58

//RateChange klaidu kodai
#define	ERROR_RATECHANGE_OPENING_DB_FILE -60
#define	ERROR_RATECHANGE_DETECTING_DB_FILE_SIZE -61
#define	ERROR_RATECHANGE_UNEVEN_DB_FILE_SIZE -62
#define	ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_DB -63
#define	ERROR_RATECHANGE_READING_DB_FILE -64
#define	ERROR_RATECHANGE_UNFINISHED_READING_DB_FILE -65
#define	ERROR_RATECHANGE_OPENING_DB_FON_WEIGHTS_FILE -66
#define	ERROR_RATECHANGE_DETECTING_DB_FON_WEIGHTS_FILE_SIZE -67
#define	ERROR_RATECHANGE_READING_DB_FON_WEIGHTS_FILE -68
#define	ERROR_RATECHANGE_UNFINISHED_READING_DB_FON_WEIGHTS_FILE -69
#define	ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PHONEME_LIST -70
#define	ERROR_RATECHANGE_UNSPECIFIED_PHONEME_LENGTH -71
#define	ERROR_RATECHANGE_NONNUMERICAL_PHONEME_LENGTH -72
#define	ERROR_RATECHANGE_OPENING_DB_PIK_FILE -73
#define	ERROR_RATECHANGE_DETECTING_DB_PIK_FILE_SIZE -74
#define	ERROR_RATECHANGE_READING_DB_PIK_FILE -75
#define	ERROR_RATECHANGE_UNFINISHED_READING_DB_PIK_FILE -76
#define	ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PIK_LIST -77
#define	ERROR_RATECHANGE_NONNUMERICAL_PIK_VALUE -78
#define	ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_PIK_INFO -79

#define	ERROR_RATECHANGE_MEMORY_ALLOCATION_FOR_CHANGED_RATE_DB -80
#define	ERROR_RATECHANGE_SIGNAL_BUFFER_OVERFLOW -81

//LithUSS klaidu kodai
#define	ERROR_LITHUSS_LOADING_TEXTNORMALIZATION_DLL -19
#define	ERROR_LITHUSS_LOADING_TRANSCRLUSS_DLL -34
#define	ERROR_LITHUSS_OPENING_FACTORS_FILE -37
#define	ERROR_LITHUSS_READING_FACTORS_FILE -38
#define	ERROR_LITHUSS_LOADING_UNITSELECTION_DLL -39
#define	ERROR_LITHUSS_LOADING_RATECHANGE_DLL -59

#define	ERROR_LITHUSS_MEMORY_ALLOCATION -15
#define	ERROR_LITHUSS_MEMORY_REALLOCATION -16
#define	ERROR_LITHUSS_EVENTS_ARRAY_OVERFLOW -17

#endif //!LITHUSS_ERROR_H_INCLUDED
