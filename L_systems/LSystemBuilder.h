#ifndef LSYSTEMBUILDER_H
#define LSYSTEMBUILDER_H

#include <string>
#include <vector>
#include <tuple>
#include <iostream>

struct Production {
    std::string input; //a symbol A, B, C, etc. followed by (param1, param2, ...). For exmaple, B(param1, param2, ...).
    std::string output; //a string where each argument of the input has the form "%n", where n is the 0-indexed arg
    bool has_parameters;
    int num_parameters;
};

struct Parameter {
    std::string symbol; //some lowercase letter
    float value; //the constant value that the symbol represents in the L-System string
};


class LSystemBuilder
{
public:
    LSystemBuilder(int iterations);
    ~LSystemBuilder();

    /**
     * Begins with the axiom and builds up the L-System string for num_iterations
     * iterations. When this function finds an uppercase letter symbol A, B, C, etc., it
     * finds the appropriate production for replacing the symbol and its paramters in the
     * string. Then, the funtion calls replace. This function also searches through the
     * current string at each iterations and replaces parameter symbols with actual numbers,
     * using the parameters list.
     */
    std::string buildString();

    void setAxiom(std::string str);
    void addProduction(std::string input, std::string output, bool has_parameters, int num_parameters);
    void addParameter(std::string symbol, float value);

private:
    std::string axiom;
    std::vector<Production> productions; //the rules (Productions) for creating the next string
    std::vector<Parameter> parameters; //the constants represented by symbols in the string
    std::vector<std::string> production_inputs; //list of the input symbols to the productions
    int num_iterations;

    /**
     * This helper function takes in the input_segment and the appropriate Production for
     * that input segment, and produces the output using the production.
     */
    std::string replace(std::string input_segment, Production production);

    /**
     * This helper function finds the first occurrence of a symbol in production_inputs
     * in str.
     */
    std::tuple<int, Production> findFirstOccurrence(std::string str);

    std::string replaceParameterSymbols(std::string);

};

#endif // LSYSTEMBUILDER_H
