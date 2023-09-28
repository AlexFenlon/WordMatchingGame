#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

#define DICT_SIZE 5757
#define WORD_SIZE 5
#define MAX_GUESSES 6

int num_letters_in_word(const char *word) {
    //this function checks how many letters are in a word
    int count = 0;
    for (int i = 0; i < WORD_SIZE; i++) {
        if (word[i] != '\0') {
            count++;
        }
    }
    return count;
}

int letter_in_word(const char *arr, char letter, int size) {
    //this function checks if a letter is in the word
    for (int i = 0; i < size; i++) {
        if (arr[i] == letter) {
            return 1;
        }
    }
    return 0;
}

int is_valid_word(const char word[]) {
    //this function makes sure the word is in the word list and is 5 letters
    char dictionary[DICT_SIZE][WORD_SIZE + 1];
    load_word_list(dictionary);
    int word_length = 0;
    while (word[word_length] != '\0') {
        word_length++;
    }
    if (word_length != WORD_SIZE) {
        return 0; // word has the wrong length
    }
    for (int i = 0; i < DICT_SIZE; i++) {
        int is_match = 1;
        for (int j = 0; j < WORD_SIZE; j++) {
            if (dictionary[i][j] != word[j] && dictionary[i][j] != word[j] - 32 && dictionary[i][j] != word[j] + 32) {
                is_match = 0;
                break;
            }
        }
        if (is_match) {
            return 1; // word is valid
        }
    }
    return 0; // word is not in the loaded word list
}

int main() {
    //this loads the word list so the program can use it
    char dictionary[DICT_SIZE][WORD_SIZE + 1];
    load_word_list(dictionary);
    //these variables are being initialized for use in stats later
    int num_games = 0;
    int num_games_won = 0;
    int total_alphabet_used = 0;
    //a while loop to keep playing the game until the user presses q to quit
    while (1) {
        num_games++;
        srand(time(NULL)); // this seeds rand which means it picks a different random number each boot
        //this code gemerates a random index and copies the characters of the randomly selected word in the list
        int random_idx = rand() % DICT_SIZE;
        char target_word[WORD_SIZE + 1];
        for (int i = 0; i < WORD_SIZE; i++) {
            target_word[i] = dictionary[random_idx][i];
        }
        target_word[WORD_SIZE] = '\0';
        //this creates the ----- for a non guessed word.
        char guessed_word[WORD_SIZE + 1];
        for (int i = 0; i < WORD_SIZE; i++) {
            guessed_word[i] = '-';
        }
        guessed_word[WORD_SIZE] = '\0';
        //thus keeps track of the letters that the user has used
        char correct_letters[WORD_SIZE + 1];
        for (int i = 0; i < WORD_SIZE; i++) {
            correct_letters[i] = '\0';
        }
        int game_won = 0;
        int num_guesses = 0;
        int alphabet_count = 0;
        int alphabet[26] = {0};
        while (num_guesses < MAX_GUESSES) {
            // this prints how many guesses through the user is out of the total
            printf("\tGuess %d of %d: %s\n", num_guesses + 1, MAX_GUESSES, guessed_word);
            char guess[WORD_SIZE + 1];
            int valid_input = 0;
            while (!valid_input) {
                printf("Enter your guess (5 letters):");
                int c, i = 0;
                //counts unique number of letters entered by user
                if (!letter_in_word(correct_letters, guess[i], WORD_SIZE)) {
                    correct_letters[num_letters_in_word(correct_letters)] = guess[i];
                    alphabet[guess[i] - 'a'] = 1;
                    alphabet_count++;
                }
                // this reads each character in until a new line and  checks if it is upper or lowercase letter and converts all letters to lowercase to work
                while ((c = getchar()) != '\n' && i < WORD_SIZE) {
                    if (c >= 'A' && c <= 'Z') {
                        c += 'a' - 'A';
                    }
                    guess[i++] = c; // the character is then stored in guess array and goes up by one each time for the word
                }
                guess[i] = '\0';
                if (is_valid_word(guess)) {
                    //this checks if the word array guess is valid using the function and makes it a valid input and increases the counter and if its not valid, it prints an error
                    valid_input = 1;
                    num_guesses++;
                } else {
                    printf("\nInvalid input! Please enter a 5 letter word that appears in the word list.\n");
                }
            }
            int correct_guess = 1;
            for (int i = 0; i < WORD_SIZE; i++) {
                // this compares each user guess with the random word from the word list, if any letter in the players guess doesnt match, it is not a correct guess and breaks
                if (guess[i] != target_word[i]) {
                    correct_guess = 0;
                    break;
                }
            }
            if (correct_guess) {
                printf("\nCongratulations! You guessed the word!\n");
                num_games_won++;
                game_won = 1;
                break;
            }
            int num_correct_letters = 0;
            int num_correct_pos = 0; // number of correct positions of each letter
            for (int i = 0; i < WORD_SIZE; i++) {
                //checks guess against target_word to figure out position of each correct letter
                if (guess[i] == target_word[i]) {
                    guessed_word[i] = guess[i];
                    num_correct_pos++;
                } else {
                    for (int j = 0; j < WORD_SIZE; j++) {
                        //this checks how many correct letters the user guessed in the wrong position
                        if (guess[i] == target_word[j] && guess[i] != guessed_word[j] &&
                            !letter_in_word(correct_letters, guess[i], num_correct_letters)) {
                            correct_letters[num_correct_letters] = guess[i];
                            num_correct_letters++;
                            break;
                        }
                    }
                }
            }
            printf("\nYou guessed %d letters correctly in the wrong position:", num_correct_letters);
            for (int i = 0; i < num_correct_letters; i++) {
                printf(" %c", correct_letters[i]);
            }
            printf("\n");
        }
        total_alphabet_used += alphabet_count;
        if (!game_won) {
            //if user lost game it would print this
            printf("\nSorry, you ran out of guesses. The word was %s.\n", target_word);
        }
        printf("Press 'q' to quit or any other key to continue.\n");    int c;
        while ((c = getchar()) == '\n');
        if (c == 'q') {
            break;
        }
    }
    // prints stats of games when user presses quit
    printf("\nGuess statistics:\n");
    printf("\n\nTotal games played: %d\n", num_games);
    printf("Total games won: %d\n", num_games_won);
    printf("Win percentage: %.2f%%\n", ((float)num_games_won / num_games) * 100);
    float avg_alphabet_used = (float) total_alphabet_used / num_games / 26 * 100;
    printf("Average percentage of characters in the alphabet used in each game: %.2f%%\n", avg_alphabet_used);
    return 0;
}
