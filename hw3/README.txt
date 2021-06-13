# README.txt
# -------------------------------------------------------------------------------------------------
# course:   System Programming
# project:  (#3) Travel Monitor (sockets, threads)
# name:     George Tservenis
# email:    sdi1500164@di.uoa.gr
# -------------------------------------------------------------------------------------------------

> Παράδειγμα εκτέλεσης:

    - Compilation:
        $ make all

    - Run bash scripts:
        $ cd scripts
        $ bash testFile.sh ../resources/viruses.txt ../resources/countries.txt 10000 0
        $ bash create_infiles.sh ../resources/records.txt ../input_dir 4
        $ cd ../

    - Execution:
        $ ./travelMonitorClient -m <numMonitors> -b <socketBufferSize> -c <cyclicBufferSize> \
        $ -s <sizeOfBloom> -i <input_dir> -t <numThreads>


> Scripts:

    testFile.sh:
        - Περιγραφή:
            Δημιουργία αρχείου εγγραφών σύμφωνα με τις οδηγίες της 1η Εργασίας.
        - Εκτέλεση:
            $ bash ./testFile.sh <virusesFile> <countriesFile> <numOfLines> <duplicatesAllowed>

    create_infiles.sh:
        - Περιγραφή:
            Δημιουργία καταλόγου εισόδου σύμφωνα με τις οδηγίες της 2ης Εργασίας.
        - Εκτέλεση:
            $ bash ./create_infiles.sh <inputFile> <input_dir> <numFilesPerDirectory>

    my_bash.sh:
        - Περιγραφή:
            Δημιουργία ενός αρχείου με travel requests. Το χρησιμοποιήσα για testing.
            Δεν απαιτείται για την εκτέλεση της εργασίας.


> Δομές Δεδομένων:
    Το μεγαλύτερο ποσοστό των δομών είναι ίδιες με αυτές που απαιτούνταν για την 2η Εργασία.


> Sockets:
    Η επικοινωνία μεταξύ του Client και των Server είναι παρόμοια με εκείνη της 2ης Εργασίας μόνο
    που τώρα αντί για pipes γίνεται μέσω sockets. O Client και οι Server κρατούν μερικά δεδομένα για
    την socket-σύνδεση.


> Threads:
    Για τους σκοπούς της εργασίας υλοποιήθηκε ένα thread pool το οποίο χρησιμοποιεί μια ουρά
    υλοποιημένη ως κυκλικό buffer με χωρητικότητα η οποία ορίζεται από τον χρήστη. Για την αποφυγή
    του busy-waiting έχουν χρησιμοποιηθεί mutex locks και condition variables.

    To thread pool δημιουργείται για κάθε Server πριν πάρει τα input directories από τον Client μέσω
    του δικτύου επικοινωνίας. Τα threads δημιουργούνται και περιμένουν να μπει κάτι στην ουρά
    (κυκλικό buffer). Μόλις μπει ένα task, κάποιο thread θα ξυπνήσει και θα το αναλάβει.

    Η εισαγωγή ενός task στην ουρά γίνεται "επιμονετικά". Όσο η ουρά του thread pool είναι γεμάτη, η
    συνάρτηση thp_push_task θα περιμένει (με cond variable) να υπάρξει διαθέσιμος χώρος.

    Τα threads παραμένουν ζωντανά αλλά κοιμισμένα και μετά το στάδιο αρχικοποίησης ώστε να
    εκτελέσουν κάποιο task που μπορεί να μπει από τον Server για την εντολή /addVaccinationRecords.

    Για την καλύτερη λειτουργία του Server υλοποιήθηκε η συνάρτηση thp_block_until_done η οποία
    μπλοκάρει το αρχικό thread μέχρι τα υπόλοιπα να τελειώσουν όλα τα tasks που έχουν εισαχθεί στην
    ουρά.

    Χρησιμοποιήθηκε mutex lock για το race condition των δεδομένων του Server που είναι
    κοινόχρηστα στα threads του.

    Το thread pool καταστρέφεται μαζί με τα υπόλοιπα δεδομένα που χρησιμοποιεί ο Server.
