/*
Copyright 2021 Bruno VERCHÈRE

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
 * \file treestruct.h
 * \brief Tree creator from a formated CSV file. It write its results
 * as a LaTeX Beamer file which can be compiled with pdflatex.
 * \author Bruno Verchère
 * \version 1.0.0
 * \date 2021
 * \image html pdf_example.png "Example of a PDF output" width=50%
*/
#if !defined(TREESTUCT_H)
#define TREESTUCT_H

/**
 * \var int MAXDEPTH
 * \brief A macro representing the maximum depth of the TEX tree.
 */

/**
 * \var int MAXLEN
 * \brief A macro representing the maximum lenght of a string.
 */

/**
 * \var char* CSVSEP
 * \brief A macro representing the CSV separator (by default a semi-colon).
 */

/**
 * \var char* CSVEOL
 * \brief A macro representing the CSV end of line (by default a semi-colon and a carriage return).
 */

/**
 * \var char* LOGS
 * \brief A macro representing the path to the log file.
 */

/**
 * \var char* SRC
 * \brief A macro representing the path to the CSV data file.
 */

/**
 * \var char* TEX
 * \brief A macro representing the path to the build TEX file.
 */

#define MAXDEPTH 5
#define MAXLEN 255
#define CSVSEP ";"
#define CSVEOL ";\n"
#define LOGS "logs/treeBuilder.log"
#define SRC "data.csv"
#define TEX "tree.tex"


/**
 * \typedef Person
 * \brief The type definition of an individual.
 * \see person
 */

/**
 * \typedef Tree
 * \brief The type definition of the binary tree.
 * \see person
 */

/**
 * \struct person
 * \brief A structure representing an indivual as a tree node.
 * 
 */
typedef struct person{
    int id; /*!< The node identifier. */
    char name[MAXLEN]; /*!< The individual name. */
    int value; /*!< The individual genetic value which set the node color. */
    struct person *lc, *rc;
}Person, *Tree;


/**
 * \fn Tree createNode(int id, char name[], int value)
 * \brief Create a new indivudal (node).
 * \param id The node identifier.
 * \param name A string representing the individual name.
 * \param value The node value (representing the genetic character).
 * \return A new node.
 */
Tree createNode(int id, char name[], int value);

/**
 * \fn int buildTree(Tree *t, int id, char name[], int value, FILE* logs)
 * \brief Build the tree by adding nodes to it.
 * \param t A pointer to the tree which is currently built.
 * \param id The identifier of the node to add to the tree.
 * \param name A string representing the individual name.
 * \param value The node value (representing the genetic character).
 * \param logs The log file.
 * \return The exit code which is equal to 0 if all went well.
 */
int buildTree(Tree *t, int id, char name[], int value, FILE* logs);

/**
 * \fn getFields(char line[], char* tokens[])
 * \brief Read a CSV file row.
 * \param line The line to parse.
 * \param tokens The parsed values of this line.
 */
void getFields(char line[], char* tokens[]);

/**
 * \fn int toInt(char* str)
 * \brief Convert a string to an integer.
 * \param str The string to convert.
 * \return The converted string as an integer.
 */
int toInt(char* str);

/**
 * \fn readCSV(FILE* file, Tree *t, FILE* logs)
 * \brief Read a CSV file representing a tree.
 * \param file The CSV file.
 * \param t The tree to build from the CSV.
 * \param logs The log file.
 * \return The exit code which is equal to 0 if all went well.
 */
int readCSV(FILE* file, Tree *t, FILE* logs);

/**
 * \fn displayPerson(Tree t)
 * \brief Display a tree in the standard output.
 * \param t The tree to display
 */
void displayPerson(Tree t);

/**
 * \fn rowMissingChildren(FILE* file, int children)
 * \brief Add missing children to the tree in the TEX file.
 * \param file The TEX file to write.
 * \param children The number of missing children to add.
 */
void rowMissingChildren(FILE* file, int children);

/**
 * \fn missingChildren(int id, FILE* file, int rootId)
 * \brief Compute the number of missing children for the given node according to the root id.
 * \param id The current identifider.
 * \param file The TEX file to write.
 * \param rootId The tree root identifier.
 */
void missingChildren(int id, FILE* file, int rootId);

/**
 * \fn color(int value, char strColor[])
 * \brief Set the node color according to its value.
 * \param value The current node value.
 * \param strColor The node color as a string.
 */
void color(int value, char strColor[]);

/**
 * \fn writeLatexPerson(FILE *file, Tree t, int depth, int rootId)
 * \brief Write an individual as a node in the TEX file.
 * \param file The TEX file.
 * \param t The binary tree.
 * \param depth The current node depth.
 * \param rootId The tree root identifier.
 */
void writeLatexPerson(FILE *file, Tree t, int depth, int rootId);

/**
 * \fn writeLatexFile(FILE* file, Tree t, int rootId)
 * \brief Write a TEX file from a binary tree.
 * \param file The TEX file to write.
 * \param t The binary tree.
 * \param rootId The tree root identifier.
 */
void writeLatexFile(FILE* file, Tree t, int rootId);

/**
 * \fn writeLogs(FILE* logs, char message[])
 * \brief Add a line to the log file.
 * \param logs The log file.
 * \param message The message to add to the logs.
 */
void writeLogs(FILE* logs, char message[]);

/**
 * \fn formateInteger(int val, char ch[], char addOn[])
 * \brief Formate an integer to a specific string for the logs.
 * \param val The integer to format.
 * \param ch The logs string.
 * \param addOn The string to place before the integer.
 */
void formateInteger(int val, char ch[], char addOn[]);

#endif // TREESTUCT_H