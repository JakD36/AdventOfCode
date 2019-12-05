#include <stdio.h>
#include <vector>
#include <set>

using namespace std;
struct op;
typedef op* p_op;

int RunIntCode(int* intCode);

p_op Add(p_op* code, p_op a, p_op b);
p_op AddSymbolToVal(p_op* code, p_op symbol, p_op val);
p_op AddValToOp(p_op* code, p_op val, p_op oper);
p_op AddValToVal(p_op* code, p_op a, p_op b);
p_op AddSymbolToSymbol(p_op* code, p_op a, p_op b);
p_op AddSymbolToOp(p_op* code, p_op symbol, p_op oper);
p_op AddOpToOp(p_op* code, p_op a, p_op b);

p_op Multiply(p_op* code, p_op a, p_op b);
p_op MulSymbolToVal(p_op* code, p_op symbol, p_op val);
p_op MulValToOp(p_op* code, p_op val, p_op oper);
p_op MulValToVal(p_op* code, p_op a, p_op b);
p_op MulSymbolToSymbol(p_op* code, p_op a, p_op b);
p_op MulSymbolToOp(p_op* code, p_op symbol, p_op oper);
p_op MulOpToOp(p_op* code, p_op a, p_op b);

bool HasChildren(p_op code);

void PrintEquation(p_op* code, p_op node);

struct op
{
    int dest;
    int operation;
    bool hasValue;
    p_op children[2];
};

int main()
{
    char filepath[] = "Day2/input.txt";
    FILE* file = fopen(filepath,"r");
    
    vector<int> memory;
   
    int val;
    while(fscanf(file,"%d",&val) != EOF)
    {            
        memory.push_back(val);
        fscanf(file,",");
    }    

    p_op *code = new p_op[memory.size()];
    
    for(int i = 0; i < memory.size(); ++i)
    {
        code[i] = new op();
        code[i]->hasValue = true;
        code[i]->dest = memory[i];
    }

    code[1] = new op();
    code[1]->hasValue = false;
    code[1]->dest = 0;

    code[2] = new op();
    code[2]->hasValue = false;
    code[2]->dest = 0;

    // Build new opcode graph
    // Collect old ops
    int i = 0;
    while(code[i]->dest != 99)
    {
        switch (code[i]->dest)
        {
        case 1:
            code[code[i+3]->dest] = Add(code,code[i+1],code[i+2]);
            
            break;
        case 2:
            code[code[i+3]->dest] = Multiply(code, code[i+1],code[i+2]);
            break;
        }
        if(!code[code[i+3]->dest]->hasValue)
        {
            printf("%d = ",code[i+3]->dest);
            PrintEquation(code,code[code[i+3]->dest]);
            printf("\n");
        }
        i+=4;
    }

    if(code[0]->hasValue)
    {
        printf("0 has value\n");
    }
    else
    {
        printf("0 has no value\n");
    }
    if(HasChildren(code[0]))
    {
        printf("0 has children\n");
    }
    else
    {
        printf("0 has no children\n");
    }

    i = 19;
    printf("%d = ",i);
    PrintEquation(code,code[i]);
    printf("\n");
    
    // printf("%d\n",code[0]->dest);
    // delete [] code;
    // int output = RunIntCode(intCode.data());
    // printf("%d\n",output);
    return 0;
}

#pragma region Addition
p_op Add(p_op* code, p_op a, p_op b)
{
    if(a->hasValue && b->hasValue) 
    {
        return AddValToVal(code,a,b);
    }
    else if(a->hasValue && !b->hasValue)
    {
        if(HasChildren(b))
        {
            return AddValToOp(code,a,b);
        }
        else
        {
            return AddSymbolToVal(code,b,a);
        }
    }
    else if(!a->hasValue && b->hasValue)
    {
        if(HasChildren(a))
        {
            return AddValToOp(code,b,a);
        }
        else
        {
            return AddSymbolToVal(code,a,b);
        }
    }
    else // Neither A or b have vals could be symbols could be operations of symbols
    {
        bool aOp = HasChildren(a);
        bool bOp = HasChildren(b);
        
        if(aOp && bOp) // a is op b is op
        {
            return AddOpToOp(code,a,b);
        }
        else if(!aOp && bOp) // a is symbol b is op
        {
            return AddSymbolToOp(code,a,b);
        }
        else if(aOp && !bOp) // a is op b is symbol
        {
            return AddSymbolToOp(code,b,a);
        }
        else // a is symbol b is symbol
        {
            return AddSymbolToSymbol(code,a,b);
        }
    }
}

p_op AddSymbolToVal(p_op* code, p_op symbol, p_op val)
{
    p_op out = new op();
    out->children[0] = symbol;
    out->children[1] = val;
    out->operation = 1;
    out->hasValue = false;
    return out;
}

p_op AddValToOp(p_op* code, p_op val, p_op oper)
{
    p_op out = new op();
    
    if(oper->operation == 1) // Addition
    {
        if(oper->children[0]->hasValue) 
        {
            out->children[0] = Add(code,val,oper->children[0]); // Add the value child
            out->children[1] = oper->children[1];
            out->operation = 1;
            out->hasValue = false;
            return out;
        }
        else if(oper->children[1]->hasValue)
        {
            out->children[0] = Add(code,val,oper->children[1]); // Add the value child
            out->children[1] = oper->children[1];
            out->operation = 1;
            out->hasValue = false;
            return out;
        }
        else
        {
            // TODO Be more specific with this func
            if(HasChildren(oper->children[0]) && HasChildren(oper->children[1]))
            {
                if(oper->children[0]->operation == 1)
                {
                    return AddValToOp(code,val,oper->children[0]);
                }
                else if(oper->children[1]->operation == 1)
                {
                    return AddValToOp(code,val,oper->children[1]);
                }
                else if(oper->children[0]->operation == 2)
                {

                }
                else if(oper->children[2]->operation == 2)
            }
            AddValToOp(val,oper)
            out->children[0] = oper;
            out->children[1] = val;
            out->operation = 1;
            out->hasValue = false;
            return out;
        }
    }
    else if(oper->operation == 2)
    {
        out->children[0] = oper;
        out->children[1] = val;
        out->operation = 1;
        out->hasValue = false;
        return out;
    }
    
    return NULL;
}

p_op AddValToVal(p_op* code, p_op a, p_op b)
{
    p_op out = new op();
    
    if(code[a->dest]->hasValue && code[b->dest]->hasValue)
    {
        out->hasValue = true;
        out->dest = code[a->dest]->dest + code[b->dest]->dest;
    }
    else
    {
        out = Add(code,code[a->dest],code[b->dest]);
    }
    
    // else if(code[a->dest]->hasValue && !code[b->dest]->hasValue)
    // {
    //     if(HasChildren(code[b->dest]))
    //     {
    //         out = AddValToOp(code,code[a->dest],code[b->dest]);
    //     }
    //     else
    //     {
    //         out = AddSymbolToVal(code,code[b->dest],code[a->dest]);
    //     }
    // }
    // else if(!code[a->dest]->hasValue && code[b->dest]->hasValue)
    // {
    //     if(HasChildren(code[a->dest]))
    //     {
    //         out = AddValToOp(code,code[b->dest],code[a->dest]);
    //     }
    //     else
    //     {
    //         out = AddSymbolToVal(code,code[a->dest],code[b->dest]);
    //     }
    // }
    // else
    // {
        
    // }

    return out;
}

p_op AddSymbolToSymbol(p_op* code, p_op a, p_op b)
{
    p_op out = new op();
    out->children[0] = a;
    out->children[1] = b;
    out->operation = 1;
    out->hasValue = false;
    return out;
}

p_op AddSymbolToOp(p_op* code, p_op symbol, p_op oper)
{
    if(oper->operation == 1) // Addition
    {
        if(oper->children[0] == symbol)
        {
            p_op out = new op();
            p_op child = new op();
            p_op valChild = new op();

            valChild->dest = 2;
            valChild->hasValue = true;

            child->children[0] = symbol;
            child->children[1] = valChild;
            child->hasValue = false;
            child->operation = 2;

            out->children[0] = oper->children[1];
            out->children[1] = child;
            out->hasValue = false;
            out->operation = 2;

            return out;
        }
        else if(oper->children[1] == symbol)
        {
            p_op out = new op();
            p_op child = new op();
            p_op valChild = new op();

            valChild->dest = 2;
            valChild->hasValue = true;

            child->children[0] = symbol;
            child->children[1] = valChild;
            child->hasValue = false;
            child->operation = 2;

            out->children[0] = oper->children[0];
            out->children[1] = child;
            out->hasValue = false;
            out->operation = 2;

            return out;
        }
        else
        {
            p_op out = new op();
            out->children[0] = symbol;
            out->children[1] = oper;
            out->hasValue = false;
            out->operation = 1;
            return out;
        }
    }
    else // Multiplication
    {
        if(oper->children[0] == symbol)
        {
            if(oper->children[1]->hasValue)
            {
                oper->children[1]->dest += 1;
            }
            else
            {
                p_op child = new op();
                p_op valChild = new op();
                valChild->dest = 1;
                valChild->hasValue = true;

                child->children[0] = oper->children[1];
                child->children[1] = valChild; //TODO verify this is fine, might be a misstep to say its 1
                child->operation = 1;
                child->hasValue = false;

                oper->children[1] = child;
            }
            return oper;    
        }
        else if(oper->children[1] == symbol)
        {
            if(oper->children[0]->hasValue)
            {
                oper->children[0]->dest += 1;
            }
            else
            {
                p_op child = new op();
                p_op valChild = new op();
                valChild->dest = 1;
                valChild->hasValue = true;

                child->children[0] = oper->children[0];
                child->children[1] = valChild; //TODO verify this is fine, might be a misstep to say its 1
                child->operation = 1;
                child->hasValue = false;

                oper->children[0] = child;
            }
            return oper;    
        }
        else // Neither 
        {
            p_op out = new op();
            out->hasValue = false;
            out->operation = 1;
            out->children[0] = oper;
            out->children[1] = symbol;
            return out;
        }
    }
    return NULL;
}

p_op AddOpToOp(p_op* code, p_op a, p_op b)
{
    if(a->operation == 1 && b->operation == 1) // a is add b is add
    {
        // We can add any vals, add any common symbols, create operation for rest
        p_op out = new op();
        if(a->children[0]->hasValue)
        {
            out = AddValToOp(code,a->children[0],b);
        }
        else
        {
            out = AddSymbolToOp(code,a->children[0],b);
        }

        if(a->children[1]->hasValue)
        {
            out = AddValToOp(code,a->children[1],out);
        }
        else
        {
            out = AddSymbolToOp(code,a->children[1],out);
        }
        return out;
    }
    else if(a->operation == 1 && b->operation == 2) // a is add b is mult
    {
        p_op out = new op();
        if(a->children[0]->hasValue)
        {
            out = AddValToOp(code,a->children[0],b);
        }
        else
        {
            out = AddSymbolToOp(code,a->children[0],b);
        }

        if(a->children[1]->hasValue)
        {
            out = AddValToOp(code,a->children[1],out);
        }
        else
        {
            out = AddSymbolToOp(code,a->children[1],out);
        }
        return out;
    }
    else if(a->operation == 2 && b->operation == 1) // a is mult b is add
    {
        p_op out = new op();
        if(b->children[0]->hasValue)
        {
            out = AddValToOp(code,b->children[0],a);
        }
        else
        {
            out = AddSymbolToOp(code,b->children[0],a);
        }

        if(b->children[1]->hasValue)
        {
            out = AddValToOp(code,b->children[1],out);
        }
        else
        {
            out = AddSymbolToOp(code,b->children[1],out);
        }
        return out;
    }
    else if(a->operation == 2 && b->operation == 2) // a is mult b is mult
    {
        p_op out = new op();
        out->hasValue = false;
        out->operation = 1;
        out->children[0] = a;
        out->children[0] = b;
        return out;
    }
}
#pragma endregion

#pragma region Multiplication
p_op Multiply(p_op* code, p_op a, p_op b)
{
    if(a->hasValue && b->hasValue) 
    {
        return MulValToVal(code,a,b);
    }
    else if(a->hasValue && !b->hasValue)
    {
        if(HasChildren(b))
        {
            return MulValToOp(code,a,b);
        }
        else
        {
            return MulSymbolToVal(code,b,a);
        }
    }
    else if(!a->hasValue && b->hasValue)
    {
        if(HasChildren(a))
        {
            return MulValToOp(code,b,a);
        }
        else
        {
            return MulSymbolToVal(code,a,b);
        }
    }
    else // Neither A or b have vals could be symbols could be operations of symbols
    {
        bool aOp = HasChildren(a);
        bool bOp = HasChildren(b);
        
        if(aOp && bOp) // a is op b is op
        {
            return MulOpToOp(code,a,b);
        }
        else if(!aOp && bOp) // a is symbol b is op
        {
            return MulSymbolToOp(code,a,b);
        }
        else if(aOp && !bOp) // a is op b is symbol
        {
            return MulSymbolToOp(code,b,a);
        }
        else // a is symbol b is symbol
        {
            return MulSymbolToSymbol(code,a,b);
        }
    }
}

p_op MulSymbolToVal(p_op* code, p_op symbol, p_op val)
{
    p_op out = new op();
    out->hasValue = false;
    out->operation = 2;
    out->children[0] = symbol;
    out->children[1] = val;
    return out;
}

p_op MulValToOp(p_op* code, p_op val, p_op oper)
{
    if(HasChildren(oper->children[0]))
    {
        oper->children[0] = MulValToOp(code, val,oper->children[0]);
    }
    else
    {
        oper->children[0] = MulSymbolToVal(code, oper->children[0],val);
    }

    if(HasChildren(oper->children[1]))
    {
        oper->children[1] = MulValToOp(code, val,oper->children[1]);
    }
    else
    {
        oper->children[1] = MulSymbolToVal(code, oper->children[1],val);
    }
    return oper;
}

p_op MulValToVal(p_op* code, p_op a, p_op b)
{
    p_op out = new op();
    
    if(code[a->dest]->hasValue && code[b->dest]->hasValue)
    {
        out->hasValue = true;
        out->dest = code[a->dest]->dest * code[b->dest]->dest;
    }
    else
    {
        out = Multiply(code,code[a->dest],code[b->dest]);
    }
    return out;
}

p_op MulSymbolToSymbol(p_op* code, p_op a, p_op b)
{// Ignoring Quadratic case as that adds complexity currently not needed
    p_op out = new op();
    out->hasValue = false;
    out->operation = 2;
    out->children[0] = a;
    out->children[0] = b;
    return out;
}

p_op MulSymbolToOp(p_op* code, p_op symbol, p_op oper)
{
    if(HasChildren(oper->children[0]))
    {
        oper->children[0] = MulSymbolToOp(code,symbol,oper->children[0]);
    }
    else
    {
        oper->children[0] = MulSymbolToSymbol(code,oper->children[0],symbol);
    }

    if(HasChildren(oper->children[1]))
    {
        oper->children[1] = MulSymbolToOp(code,symbol,oper->children[1]);
    }
    else
    {
        oper->children[1] = MulSymbolToSymbol(code,oper->children[1],symbol);
    }
    return oper;
}

p_op MulOpToOp(p_op* code, p_op a, p_op b)
{
    if(a->operation == 1 && b->operation == 1) // a is add b is add
    {
        // a[0]b[0] + a[0]b[1] + a[1]b[0] + a[1]b[1]
        p_op outA;
        p_op outB;
        p_op outC;
        p_op outD;
        
        // Is a[0] symbol, operation or val
        if(a->children[0]->hasValue) // a[0] is val
        {
            outA = MulValToOp(code, a->children[0],b->children[0]);
            outB = MulValToOp(code, a->children[0],b->children[1]);
        }
        else if(HasChildren(a->children[0])) // a[0] is operation
        {
            outA = MulOpToOp(code, a->children[0],b->children[0]);
            outB = MulValToOp(code, a->children[0],b->children[1]);
        }
        else // a[0] is symbol
        {
            outA = MulSymbolToOp(code, a->children[0],b->children[0]);
            outB = MulValToOp(code, a->children[0],b->children[1]);
        }

        if(a->children[1]->hasValue) // a[1] is val
        {
            outC = MulValToOp(code, a->children[1],b->children[0]);
            outD = MulValToOp(code, a->children[1],b->children[1]);
        }
        else if(HasChildren(a->children[1])) // a[1] is operation
        {
            outC = MulOpToOp(code, a->children[1],b->children[0]);
            outD = MulValToOp(code, a->children[1],b->children[1]);
        }
        else // a[1] is symbol
        {
            outC = MulSymbolToOp(code, a->children[1],b->children[0]);
            outD = MulValToOp(code, a->children[1],b->children[1]);
        }
        
        return AddOpToOp(code, AddOpToOp(code, outA,outB),AddOpToOp(code, outC,outD));
    }
    else if(a->operation == 1 && b->operation == 2) // a is add b is mult
    {
        // a[0]b + a[1]b 
        p_op outA;
        p_op outB;
        
        // Is a[0] symbol, operation or val
        if(a->children[0]->hasValue) // a[0] is val
        {
            outA = MulValToOp(code, a->children[0],b);
        }
        else if(HasChildren(a->children[0])) // a[0] is operation
        {
            outA = MulOpToOp(code, a->children[0],b);
        }
        else // a[0] is symbol
        {
            outA = MulSymbolToOp(code, a->children[0],b);
        }

        if(a->children[1]->hasValue) // a[1] is val
        {
            outB = MulValToOp(code, a->children[1],b);
        }
        else if(HasChildren(a->children[1])) // a[1] is operation
        {
            outB = MulOpToOp(code, a->children[1],b);
        }
        else // a[1] is symbol
        {
            outB = MulSymbolToOp(code, a->children[1],b);
        }
        
        return AddOpToOp(code, outA,outB);
    }
    else if(a->operation == 2 && b->operation == 1) // a is mult b is add
    {
        // b[0]a + b[1]a 
        p_op outA;
        p_op outB;
        
        // Is b[0] symbol, operation or val
        if(b->children[0]->hasValue) // b[0] is val
        {
            outA = MulValToOp(code, b->children[0],a);
        }
        else if(HasChildren(b->children[0])) // b[0] is operation
        {
            outA = MulOpToOp(code, b->children[0],a);
        }
        else // b[0] is symbol
        {
            outA = MulSymbolToOp(code, b->children[0],a);
        }

        if(b->children[1]->hasValue) // b[1] is val
        {
            outB = MulValToOp(code, b->children[1],a);
        }
        else if(HasChildren(b->children[1])) // b[1] is operation
        {
            outB = MulOpToOp(code, b->children[1],a);
        }
        else // b[1] is symbol
        {
            outB = MulSymbolToOp(code, b->children[1],a);
        }
        
        return AddOpToOp(code, outA,outB);
    }
    else if(a->operation == 2 && b->operation == 2) // a is mult b is mult
    {
        // (a[0] * b )*a[1]*b
        if(a->children[0]->hasValue) // a[0] is val
        {
            b = MulValToOp(code, a->children[0],b);
        }
        else if(HasChildren(a->children[0])) // a[0] is operation
        {
            b = MulOpToOp(code, a->children[0],b);
        }
        else // a[0] is symbol
        {
            b = MulSymbolToOp(code, a->children[0],b);
        }

        if(a->children[1]->hasValue) // a[1] is val
        {
            b = MulValToOp(code, a->children[1],b);
        }
        else if(HasChildren(a->children[1])) // a[1] is operation
        {
            b = MulOpToOp(code, a->children[1],b);
        }
        else // a[1] is symbol
        {
            b = MulSymbolToOp(code, a->children[1],b);
        }
        
        return b;
    }
}
#pragma endregion

bool HasChildren(p_op node)
{
    return node->children[0] != NULL && node->children[1] != NULL;
}

void PrintEquation(p_op* code, p_op node)
{
    if(node->hasValue)
    {
        printf(" %d ",node->dest);
    }
    else if(node == code[1])
    {
        printf(" XXX ");
    }
    else if(node == code[2])
    {
        printf(" YYY ");
    }
    else if(HasChildren(node))
    {
        printf("( ");
        PrintEquation(code,node->children[0]);

        if(node->operation == 1)
            printf(" + ");
        else
            printf(" * ");

        PrintEquation(code,node->children[1]);
        printf(" )");
    }
}

int RunIntCode(int* intCode)
{
    int i = 0;
    while(intCode[i] != 99)
    {
        switch (intCode[i])
        {
        case 1:
            intCode[intCode[i+3]] = intCode[intCode[i + 1]] + intCode[intCode[i + 2]];
            break;
        case 2:
            intCode[intCode[i+3]] = intCode[intCode[i + 1]] * intCode[intCode[i + 2]];
            break;
        }
        i+=4;
    }
    return intCode[0];
}