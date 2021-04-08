 README.txt
---------------------------------------------------------------------------------------------------
 System Programming - Homework 1
 name:    George Tservenis
 email:   sdi1500164@di.uoa.gr
 date:    25-03-2021
---------------------------------------------------------------------------------------------------

> Inconsistencies for Citizen Records:
    To define the different types of logical inconsistencies first I differentiate the attributes
    into 3 categories: A(citizen_id), B(name surname country age), C(virus YES/NO [date]). Category
    A is just the citizen_id which serves as a unique identifier for a person, category B contains
    other attributes of that person's identity and category C's attributes describe whether that
    person has been vaccinated for a virus (and if so, when) or not.

    Rules:
    1.  Two records can have the same citizen_id, if and only if they have the same attributes
        in category B.
    2.  Two records with the same attributes in categories A and B can have the same virus

    Base on that differentiation, the types of logical inconsistencies are:
    1. Two records having the same citizen_id but any of the attributes of category B is different.
        Example 1:
            889 John Papadopoulos Greece 19 COVID-19 YES 27-12-2020
            889 Maria Tortellini Italy 36 COVID-19 YES 27-12-2020
        Example 2:
            889 John Papadopoulos Greece 19 COVID-19 YES 27-12-2020
            889 George Papadopoulos Greece 19 COVID-19 YES 27-12-2020
    2. Two records having different citizen_id but all of the attributes of category B are the same.
        Example 2:
                889 John Papadopoulos Greece 19 COVID-19 YES 27-12-2020
                902 John Papadopoulos Greece 19 SARS-1 NO
    3. Two records having the same attributes for
