#include "LSystemBuilder.h"
#include <algorithm>
#include "Settings.h"

std::vector<std::string> splitByAsterisk(std::string str) {
    std::vector<std::string> ret_vec;
    bool asterisks_remaining = true;
    std::string sub = str;
    while(asterisks_remaining) {
        int idx = sub.find("*");
        if(idx == std::string::npos) {
            asterisks_remaining = false;
        }
        ret_vec.push_back(sub.substr(0,idx));
        sub = sub.substr(std::min<int>(idx + 1, sub.length()));
    }

    return ret_vec;
}

LSystemBuilder::LSystemBuilder(int iterations) :
    num_iterations(iterations)
{
    if (settings.treetype == TT_MONOPODIAL) {
        setAxiom("A(1.0,0.1)");
        addProduction("A","!(%1)F(%0)[&(c)B(%0*b,%1*.707)]/(e)A(%0*a,%1*.707)",true, 2);
        addProduction("B","!(%1)F(%0)[-(d)$C(%0*b,%1*.707)]C(%0*a,%1*.707)",true, 2);
        addProduction("C","!(%1)F(%0)[+(d)$B(%0*b,%1*.707)]B(%0*a,%1*.707)",true, 2);
        addParameter("a", settings.param_a);
        addParameter("b", settings.param_b);
        addParameter("c", settings.param_c);
        addParameter("d", settings.param_d);
        addParameter("e", settings.param_e);
    } else if (settings.treetype == TT_SYMPODIAL) {
        setAxiom("A(1.0,0.1)");
        addProduction("A","!(%1)F(%0)[&(c)B(%0*a,%1*e)]/(180)[&(d)B(%0*b,%1*e)]",true, 2);
        addProduction("B","!(%1)F(%0)[+(c)$B(%0*a,%1*e)][-(d)$B(%0*b,%1*e)]",true, 2);
        addParameter("a", settings.param_a);
        addParameter("b", settings.param_b);
        addParameter("c", settings.param_c);
        addParameter("d", settings.param_d);
        addParameter("e", settings.param_e);
    } else if (settings.treetype == TT_TERNARY) {

        setAxiom("!(.4)F(2)/(45)A(.4)");
        addProduction("A","!(%0)F(.5)/(120)[&(c)!(%0*e)F(.5)B(%0*e*e)]/(120)[&(c)!(%0*e)F(.5)A(%0*e*e)]/(120)[&(c)!(%0*e)F(.5)A(%0*e*e)]",true, 1);
        addProduction("B","!(%0)F(.5)/(120)[&(c)!(%0*e)F(.5)B(%0*e*e)]/(180)[&(c)!(%0*e)F(.5)A(%0*e*e)]",true, 1);
        addProduction("F","F(%0*d)",true, 1);
        //addProduction("!","!(%0*e)",true, 1);
        addParameter("a", settings.param_a);
        addParameter("b", settings.param_b);
        addParameter("c", settings.param_c);
        addParameter("d", settings.param_d);
        addParameter("e", settings.param_e);

        /*
        setAxiom("F(1.0)A(1.0,0.2)");
        addProduction("A","!(%1)F(%0)/(120)[&(c)F(%0*a)A(%0*a,%1*e)]/(120)[&(c)F(%0*a)A(%0*a,%1*e)]/(120)[&(c)F(%0*a)A(%0*a,%1*e)]",true, 2);
        addParameter("a", settings.param_a);
        addParameter("b", settings.param_b);
        addParameter("c", settings.param_c);
        addParameter("d", settings.param_d);
        addParameter("e", settings.param_e);
        */
    }
}

LSystemBuilder::~LSystemBuilder() {

}

std::string LSystemBuilder::buildString() {
    std::string curr_str = axiom;
    for (int i = 0; i < num_iterations; i++) {
        //segments is the separate parts of the output string of this iteration
        //the segments will be pieced back together at the end of the iteration
        std::vector<std::string> segments;
        std::string sub = curr_str;
        while (sub.size() > 0) {
            int occurrence_idx;
            Production p;
            std::tie(occurrence_idx, p) = findFirstOccurrence(sub);
            int idx_jump;
            if (occurrence_idx >= 0) {
                //copy the symbols preceding the substring that will be replaced
                segments.push_back(sub.substr(0, occurrence_idx));
                std::string input_segment;

                //extract the substring that will be replaced
                if (p.has_parameters) {
                    idx_jump = sub.find(")", occurrence_idx) + 1; //get the substring consisting of the the part we need to replace
                    input_segment = sub.substr(occurrence_idx, idx_jump - occurrence_idx);
                } else {
                    idx_jump = occurrence_idx+1;
                    input_segment = sub.substr(occurrence_idx, 1);
                }
                //make the replacement substring
                segments.push_back(replace(input_segment, p));
            } else {
                //if we can't find any more things to replace, just add the rest to the final string
                segments.push_back(sub);
                break;
            }
            //get rid of the part of the string we have processed
            sub = sub.substr(idx_jump);
        }
        //set curr_str to the elements of segments strung together
        std::string next_str = std::string("");
        for (int i = 0; i < segments.size(); i++) {
            next_str.append(segments[i]);
        }
        curr_str = next_str;
    }

    return replaceParameterSymbols(curr_str);
}

std::string LSystemBuilder::replace(std::string input_segment, Production production) {
    std::vector<std::string> inputs;
    if (!production.has_parameters) {
        return production.output;
    } else {
        int idx = 0;
        int parameters_found = 0;
        //skip the first two characters in the input segment because they are the
        //symbol being replaced by the Production and an open parenthesis
        std::string input_segment_copy = input_segment.substr(2);
        while (parameters_found < production.num_parameters) {
            //find the substring of the input segment up to the next comma or close parenthesis
            std::string curr_segment = input_segment_copy.substr(idx);
            int comma_idx = curr_segment.find(",");
            if (comma_idx == std::string::npos) {
                int paren_idx = curr_segment.find(")");
                inputs.push_back(curr_segment.substr(0, paren_idx));
                idx += paren_idx + 1;
            } else {
                inputs.push_back(curr_segment.substr(0, comma_idx));
                idx += comma_idx + 1;
            }
            parameters_found++;
        }
    }
    std::vector<std::string> replacement_segments;
    std::string replacement = production.output;
    bool symbols_to_replace = true;
    while (symbols_to_replace) {
        //we keep looking for % symbols until they have all been replaced by the
        //real arguments of each symbol
        int param_idx = replacement.find("%");
        if (param_idx == std::string::npos) {
            //if there are no more % symbols, just add in the rest of the Production's
            //output to the final string
            replacement_segments.push_back(replacement);
            symbols_to_replace = false;
        } else {
            replacement_segments.push_back(replacement.substr(0, param_idx));
            int param_num[1];
            std::sscanf(replacement.substr(param_idx+1, 1).c_str(), "%d", param_num);
            replacement_segments.push_back(inputs[*param_num]);
            replacement = replacement.substr(param_idx+2); //jump two spots in front of the % symbol
        }
    }

    //string together all of the segments to get the final replacement string
    std::string final_string = std::string("");
    for (int i = 0; i < replacement_segments.size(); i++) {
        final_string.append(replacement_segments[i]);
    }

    return final_string;
}

std::tuple<int, Production> LSystemBuilder::findFirstOccurrence(std::string str) {
    //go through every index in str until we find an occurrence of a symbol that one
    //of the Productions tells us we must replace
    for (int i = 0; i < str.size(); i++) {
        std::string curr_symbol = str.substr(i, 1);
        for (int j = 0; j < productions.size(); j++) {
            if (curr_symbol.compare(productions[j].input) == 0) {
                return std::make_tuple(i, productions[j]);
            }
        }
    }
    return std::make_tuple(-1, Production({std::string(""), std::string("")}));
}

/**
 * NOTE: this function ONLY works when the arguments of the productions are being MULTIPLIED
 * with other arguments or parameters of the L-System. Productions with addition, subtraction,
 * or division in the output will break this function.
 */
std::string LSystemBuilder::replaceParameterSymbols(std::string str) {
    std::vector<std::string> segments;
    std::string sub = str;
    bool segments_to_replace = true;
    while (segments_to_replace) {
        //first look for an open parenthesis, which indicates that there might
        //be an expression we have to simplify. Add everything to the left of
        //the open parenthesis and the parenthesis itself in sub to the segments vector.
        int open_paren_idx = sub.find("(");
        if (open_paren_idx == std::string::npos) {
            segments_to_replace = false;
            segments.push_back(sub);
        } else {
            segments.push_back(sub.substr(0, open_paren_idx + 1));

            //remove the part of sub that we have processed (through the open parenthesis)
            sub = sub.substr(open_paren_idx + 1);

            //once we have found an open parenthesis, simplify the comma-separated
            //expressions and add the expressions to segments until we make it to
            //a close parenthesis.
            bool found_close_paren = false;
            while(!found_close_paren) {
                int comma_idx = sub.find(",");
                int close_paren_idx = sub.find(")");
                int length_to_simplify;
                //figure out whether the next comma or the next close parenthesis is closer.
                if (comma_idx == std::string::npos || comma_idx > close_paren_idx) {
                    found_close_paren = true;
                    length_to_simplify = close_paren_idx;
                } else {
                    length_to_simplify = comma_idx;
                }

                //simplify the expression and add it to the segments vector
                std::string to_simplify = sub.substr(0, length_to_simplify);
                std::vector<std::string> nums_and_parameters = splitByAsterisk(to_simplify);
                float product = 1;
                for (int i = 0; i < nums_and_parameters.size(); i++) {
                    float f = std::strtof(nums_and_parameters[i].c_str(), nullptr);
                    if (f == 0.0) {
                        for (int p_idx = 0; p_idx < parameters.size(); p_idx++) {
                            if (nums_and_parameters[i].compare(parameters[p_idx].symbol) == 0) {
                                product *= parameters[p_idx].value;
                            }
                        }
                    } else {
                        product *= f;
                    }
                }

                segments.push_back(std::to_string(product));

                if (found_close_paren) {
                    segments.push_back(")");
                } else {
                    segments.push_back(",");
                }

                //remove the part of sub that we have processed
                sub = sub.substr(length_to_simplify + 1);
            }
        }
    }

    //string the segments together to get the final simplified string
    std::string final_string = std::string("");
    for (int i = 0; i < segments.size(); i++) {
        final_string.append(segments[i]);
    }

    return final_string;
}

void LSystemBuilder::setAxiom(std::string str) {
    axiom = str;
}

void LSystemBuilder::addProduction(std::string input, std::string output, bool has_parameters, int num_parameters) {
    productions.push_back(Production({input, output, has_parameters, num_parameters}));
}

void LSystemBuilder::addParameter(std::string symbol, float value) {
    parameters.push_back(Parameter({symbol, value}));
}
