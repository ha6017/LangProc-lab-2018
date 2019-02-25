#include "ast.hpp"

#include <regex>

int32_t Interpret(
    InterpretContext &context, // Contains the parameters and variable bindings
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match(program->type, reNum) ){
        return std::atol(program->type.c_str());
    }   
    // TODO : Check for things matching reId
    else if( regex_match(program->type, reId) ){
        return context.bindings[program->type];
    }
        
    else if(program->type=="Param"){
        unsigned index=atol(program->value.c_str());
        auto value=context.params.at(index);
        return value;
        
    }else if(program->type=="Output"){
        int32_t val=Interpret(context, program->branches.at(0));
        std::cout<<val<<std::endl;
        return val;
    }   
    // TODO: Handle other constructs
    else if(program->type=="LessThan"){
        int32_t val_A=Interpret(context, program->branches.at(0));
        int32_t val_B=Interpret(context, program->branches.at(1));
        if(val_A < val_B) { return 1;}
        return 0;
    }

    else if (program->type == "Add"){
        int32_t val_A=Interpret(context, program->branches.at(0));
        int32_t val_B=Interpret(context, program->branches.at(1));
        return (val_A + val_B);
    }

    else if (program->type == "Sub"){
        int32_t val_A=Interpret(context, program->branches.at(0));
        int32_t val_B=Interpret(context, program->branches.at(1));
        return (val_A - val_B);
    }

    else if ( program->type == "Assign"){
        int32_t V = Interpret(context, program->branches.at(0));
        context.bindings[program->value] = V;
        return V;
    }

    else if (program->type == "Input"){
        int32_t x;
        std::cin>>x;
        return x;
    }

    else if (program->type == "If"){
        int32_t C = Interpret(context, program->branches.at(0));
        if(C != 0){
            //execute X
            int32_t X = Interpret(context, program->branches.at(1));
            return X;
        }
        else{
            //execute Y
            int32_t Y = Interpret(context, program->branches.at(2));
            return Y;
        }
    }

    else if(program->type == "While"){
        int32_t C = Interpret(context, program->branches.at(0));
        while(C!=0){
            int32_t X = Interpret(context, program->branches.at(1));
            //return X;
            C = Interpret(context, program->branches.at(0));
        }
        return 0;
    }

    else if(program->type == "Seq"){
        //std::cerr<<"size of branch = "<< program->branches.size();
        int32_t Shit = 0;
        for(int i=0;i<program->branches.size();i++)
        {
            Shit = Interpret(context, program->branches.at(i));
        }
        return Shit;
        /*for(unsigned i=0; i<program->branches.size(); i++){
            Interpret(destReg, program->branches[i]);
        }*/
    }
        
    else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}
