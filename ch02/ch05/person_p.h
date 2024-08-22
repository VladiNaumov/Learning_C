// File name: ExtremeC_examples_chapter8_2_person_p.h
// Description: Public interface of the person class

#ifndef PERSON_P_H
#define PERSON_P_H

// Private definition
typedef struct {
  char first_name[32];
  char last_name[32];
  unsigned int birth_year;
} person_t;

#endif
