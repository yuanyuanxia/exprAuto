#include <algorithm>
#include <queue>
#include <numeric>

#include "basic.hpp"
#include "monoInfo.hpp"
#include "expandAST.hpp"
#include "exprAuto.hpp"
#include "polyRewrite.hpp"
#include "mathfuncRewrite.hpp"
#include "simplifyExpr.hpp"

std::unique_ptr<ExprAST> combineFraction(const std::unique_ptr<ExprAST> &numerator, const std::unique_ptr<ExprAST> &denominator)
{
    // init
    std::unique_ptr<ExprAST> numeratorNew = nullptr;
    std::unique_ptr<ExprAST> denominatorNew = nullptr;
    std::unique_ptr<ExprAST> numeratorFinal = nullptr;
    std::unique_ptr<ExprAST> denominatorFinal = nullptr;
    // numerator
    if (isFraction(numerator))
    {
        const std::unique_ptr<ExprAST> numeratorTmp = getNumerator(numerator);
        const std::unique_ptr<ExprAST> denominatorTmp = getDenominator(numerator);

        numeratorNew = numeratorTmp->Clone();
        denominatorNew = mulExpr(denominator, denominatorTmp);
    }
    else
    {
        // fprintf(stderr, "\tNOTE: combineFraction: the input numerator is not a fraction.\n");
        numeratorNew = numerator->Clone();
        denominatorNew = denominator->Clone();
    }
    // denominator
    if (isFraction(denominator))
    {
        const std::unique_ptr<ExprAST> numeratorTmp = getNumerator(denominatorNew);
        const std::unique_ptr<ExprAST> denominatorTmp = getDenominator(denominatorNew);

        numeratorFinal = mulExpr(numeratorNew, denominatorTmp);
        denominatorFinal = numeratorTmp->Clone();
    }
    else
    {
        // fprintf(stderr, "\tNOTE: combineFraction: the input denominator is not a fraction.\n");
        numeratorFinal = numeratorNew->Clone();
        denominatorFinal = denominatorNew->Clone();
    }

    return divExpr(numeratorFinal, denominatorFinal);
}

std::vector<std::unique_ptr<ExprAST>> extractItems(const std::unique_ptr<ExprAST> &expr)
{
    // fprintf(stderr, "extractItems: start--------\n");
    if (expr == nullptr)
    {
        fprintf(stderr, "\tERROR: extractItems: the input expr is nullptr!\n");
        return {};
    }
    std::vector<std::unique_ptr<ExprAST>> items;
    if (expr->type() != "Binary")
    {
        std::unique_ptr<ExprAST> exprTmp = expr->Clone();
        items.push_back(std::move(exprTmp));
    }
    else
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOpPtr->getOp();

        if (op != '+')
        {
            std::unique_ptr<ExprAST> exprTmp = expr->Clone();
            items.push_back(std::move(exprTmp));
        }
        else
        {
            std::unique_ptr<ExprAST> &lhs = binOpPtr->getLHS();
            auto items1 = extractItems(lhs);
            std::unique_ptr<ExprAST> &rhs = binOpPtr->getRHS();
            auto items2 = extractItems(rhs);

            items.insert(items.end(), std::make_move_iterator(items1.begin()), std::make_move_iterator(items1.end()));
            items.insert(items.end(), std::make_move_iterator(items2.begin()), std::make_move_iterator(items2.end()));
        }
    }

    // print info for debug
    // fprintf(stderr, "\textractItems: expr size = %ld\n", items.size());
    // for (long unsigned int i = 0; i < items.size(); i++)
    // {
    //     fprintf(stderr, "\textractItems: No.%lu: %s\n", i, PrintExpression(items[i]).c_str());
    // }

    // fprintf(stderr, "extractItems: end----------\n");
    return items;
}

std::unique_ptr<ExprAST> moveDivKernel(const std::unique_ptr<ExprAST> &expr)
{                                          // level Traversal
    std::queue<BinaryExprAST *> nodeQueue; // nodeQueue存储层序遍历的节�?
    std::queue<char> sideQueue;            // sideQueue�?能存�?'L'�?'R',表示对应节点在根节点的左边还�?右边
    std::unique_ptr<ExprAST> in = expr->Clone();
    const std::string exprType = expr->type();
    if (exprType == "Binary")
    {
        BinaryExprAST *root = dynamic_cast<BinaryExprAST *>(in.get());
        std::unique_ptr<ExprAST> &rootLhs = root->getLHS();
        std::unique_ptr<ExprAST> &rootRhs = root->getRHS();

        if (in != nullptr)
            nodeQueue.push(root);

        if ((rootLhs->type()) == "Binary")
            sideQueue.push('L');
        if ((rootRhs->type()) == "Binary")
            sideQueue.push('R');

        int level = 0; // level 代表层数
        char side;
        while (!nodeQueue.empty())
        {
            int size = nodeQueue.size(); // 这里一定�?�使用固定大小size，不使用que.size()，因为que.size�?不断变化�?
            for (int i = 0; i < size; i++)
            {
                auto node = nodeQueue.front();
                nodeQueue.pop();

                if (level > 0) //�?二层开始，nodeQueue每出队一�?节点,sideQueue也会出队
                {
                    side = sideQueue.front();
                    sideQueue.pop();
                }

                if (node->getOp() == '/' && level == 0) //根节点为'/'
                {
                    return expr->Clone();
                }

                if (node->getOp() == '*')
                {
                    if (node->getLHS())
                    {
                        if (node->getLHS()->type() == "Binary")
                        {
                            BinaryExprAST *nodeLHSPtr = dynamic_cast<BinaryExprAST *>((node->getLHS()).get());
                            nodeQueue.push(nodeLHSPtr);

                            if (level == 0)
                                side = 'L';
                            if (level > 0)
                                sideQueue.push(side == 'L' ? 'L' : 'R'); //�?二层开始，每往队列nodeQueue�?添加一�?节点，都要在sideQueue�?添加一�?'L'或�?'R'

                            if (nodeLHSPtr->getOp() == '/') //新入队的节点为目标节�?
                            {
                                if (side == 'L') //�?标节点在根节点的左侧�?
                                {
                                    //换操作�??
                                    nodeLHSPtr->setOp(root->getOp());
                                    root->setOp('/');
                                    //将根节点和目标节点的左边互换
                                    root->getRHS().swap(nodeLHSPtr->getRHS());

                                    return root->Clone();
                                }
                                if (side == 'R') //�?标节点在根节点的右侧时，�? nodeLHSPtr 在node左侧�?
                                {
                                    std::unique_ptr<ExprAST> resultRight = nodeLHSPtr->getRHS()->Clone(); //'/'右子树resultRight成为result的右子树
                                    std::unique_ptr<ExprAST> tmp = nodeLHSPtr->getLHS()->Clone();         //'/'左子树根节点将成为替�?'/'的节�?

                                    node->setLHS(tmp);                                   //改变node的左子树
                                    std::unique_ptr<ExprAST> resultLeft = root->Clone(); // node左子树变化后，root将成为result左子�?

                                    return std::make_unique<BinaryExprAST>('/', std::move(resultLeft), std::move(resultRight));
                                }
                            }
                        }
                    }
                    if (node->getRHS())
                    {
                        if (node->getRHS()->type() == "Binary")
                        {
                            BinaryExprAST *nodeRHSPtr = dynamic_cast<BinaryExprAST *>((node->getRHS()).get());
                            nodeQueue.push(nodeRHSPtr);

                            if (level == 0)
                                side = 'R';
                            if (level > 0)
                                sideQueue.push(side == 'L' ? 'L' : 'R');

                            if (nodeRHSPtr->getOp() == '/')
                            {
                                if (side == 'L')
                                {
                                    nodeRHSPtr->setOp(root->getOp());
                                    root->setOp('/');
                                    root->getRHS().swap(nodeRHSPtr->getRHS());
                                    return root->Clone();
                                }
                                if (side == 'R') //�?标节点在根节点的右侧时，�? nodeRHSPtr 在node右侧�?
                                {
                                    std::unique_ptr<ExprAST> resultRight = nodeRHSPtr->getRHS()->Clone();
                                    std::unique_ptr<ExprAST> tmp = nodeRHSPtr->getLHS()->Clone();

                                    node->setRHS(tmp); //改变node的右子树
                                    std::unique_ptr<ExprAST> resultLeft = root->Clone();

                                    return std::make_unique<BinaryExprAST>('/', std::move(resultLeft), std::move(resultRight));
                                }
                            }
                        }
                    }
                }
            }
            level++;
        }
    }
    return expr->Clone();
}

std::vector<std::unique_ptr<ExprAST>> moveDiv(const std::vector<std::unique_ptr<ExprAST>> &exprs)
{
    // fprintf(stderr, "moveDiv: start--------\n");
    std::vector<std::unique_ptr<ExprAST>> items;
    std::unique_ptr<ExprAST> moveBefore, moveAfter;
    for (long unsigned int i = 0; i < exprs.size(); i++)
    {
        moveBefore = exprs.at(i)->Clone();
        moveAfter = moveDivKernel(moveBefore);
        items.push_back(std::move(moveAfter));
    }
    // print info for debug
    // fprintf(stderr, "\tmoveDiv: expr size = %ld\n", items.size());
    // for (long unsigned int i = 0; i < items.size(); i++)
    // {
    //     fprintf(stderr, "\tmoveDiv: No.%lu: %s\n", i, PrintExpression(items[i]).c_str());
    // }
    // fprintf(stderr, "moveDiv: end----------\n");
    return items;
}

std::unique_ptr<ExprAST> mergeFraction(const std::vector<std::unique_ptr<ExprAST>> &exprs)
{
    //把分子和分母分开
    std::vector<std::unique_ptr<ExprAST>> numerators;
    std::vector<std::unique_ptr<ExprAST>> denominators;
    unsigned int fractionCount = 0;
    for (long unsigned int i = 0; i < exprs.size(); i++)
    {
        std::unique_ptr<ExprAST> exprTmp = exprs.at(i)->Clone();
        std::unique_ptr<ExprAST> numeratorTmp = nullptr;
        std::unique_ptr<ExprAST> denominatorTmp = nullptr;
        if (isFraction(exprTmp))
        {
            numeratorTmp = getNumerator(exprTmp);
            denominatorTmp = getDenominator(exprTmp);
            fractionCount++;
        }
        else
        {
            numeratorTmp = std::move(exprTmp);
            denominatorTmp = std::make_unique<NumberExprAST>(1);
        }
        numerators.push_back(std::move(numeratorTmp));
        denominators.push_back(std::move(denominatorTmp));
    }
    //处理分子，进行循�?合并
    std::vector<std::unique_ptr<ExprAST>> numeratorcom;  //建立合并之后的分子数�?
    for (long unsigned int i = 0; i < exprs.size(); i++) //写在�?�?里，每循�?一次重�?一�?
    {
        std::unique_ptr<ExprAST> exprTmp_i = numerators.at(i)->Clone();
        std::unique_ptr<ExprAST> numeratorTmp = nullptr;
        numeratorTmp = std::move(exprTmp_i);
        for (long unsigned int j = 0; j < exprs.size(); j++)
        {
            if (i != j) //乘以除了原本分母的分�?
            {
                std::unique_ptr<ExprAST> exprTmp_j = denominators.at(j)->Clone();
                numeratorTmp = mulExpr(numeratorTmp, exprTmp_j);
            }
        }
        numeratorcom.push_back(std::move(numeratorTmp));
    }
    //合并分子
    std::unique_ptr<ExprAST> exprTmp = numeratorcom.at(0)->Clone();
    std::unique_ptr<ExprAST> result = std::move(exprTmp);
    for (long unsigned int i = 1; i < exprs.size(); i++)
    {
        exprTmp = numeratorcom.at(i)->Clone();
        result = addExpr(result, exprTmp);
    }
    //处理分母
    if (fractionCount > 0)
    {
        std::unique_ptr<ExprAST> denominatorTmp = denominators.at(0)->Clone();
        for (long unsigned int i = 1; i < exprs.size(); i++) //�?�?把各�?的分母变成公分母
        {
            exprTmp = denominators.at(i)->Clone();
            denominatorTmp = mulExpr(denominatorTmp, exprTmp);
        }
        result = divExpr(result, denominatorTmp);
    }
    return result;
}

std::unique_ptr<ExprAST> minusRewrite(const std::unique_ptr<ExprAST> &expr)
{
    if(expr->type() != "Binary")
    {
        return expr->Clone();
    }

    BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOpPtr->getOp();
    auto &lhs = binOpPtr->getLHS();
    auto &rhs = binOpPtr->getRHS();
    auto lhsNew = minusRewrite(lhs);
    auto rhsNew = minusRewrite(rhs);
    if(op == '-')
    {
        std::unique_ptr<ExprAST> minusOne = std::make_unique<NumberExprAST>(-1);
        rhsNew = createBinaryExpr(minusOne, rhsNew, '*');
        op = '+';
    }
    return createBinaryExpr(lhsNew, rhsNew, op);
}

std::unique_ptr<ExprAST> preprocessInit(const std::unique_ptr<ExprAST> &expr)
{
    std::unique_ptr<ExprAST> exprNew = minusRewrite(expr);
    // fprintf(stderr, "preprocessInit: after minusRewrite, exprNew = %s\n", PrintExpression(exprNew).c_str());
    if (isFraction(expr))
    {
        exprNew = exprNew->Clone();
    }
    else
    {
        // fprintf(stderr, "preprocessInit: before expandExprWrapper, exprNew = %s\n", PrintExpression(exprNew).c_str());
        std::unique_ptr<ExprAST> exprTmp = expandExprWrapper(exprNew);
        // fprintf(stderr, "preprocessInit: after expandExprWrapper, exprTmp = %s\n", PrintExpression(exprTmp).c_str());
        std::vector<std::unique_ptr<ExprAST>> exprs1 = extractItems(exprTmp);
        // fprintf(stderr, "\tpreprocessInit: after extractItems: exprs1 size = %ld\n", exprs1.size());
        // for (size_t i = 0; i < exprs1.size(); i++)
        // {
            // fprintf(stderr, "\tpreprocessInit: after extractItems: No.%lu: %s\n", i, PrintExpression(exprs1[i]).c_str());
        // }
        std::vector<std::unique_ptr<ExprAST>> exprs2 = moveDiv(exprs1);
        // fprintf(stderr, "\tpreprocessInit: after moveDiv: exprs2 size = %ld\n", exprs2.size());
        // for (size_t i = 0; i < exprs2.size(); i++)
        // {
        //     fprintf(stderr, "\tpreprocessInit: after moveDiv: No.%lu: %s\n", i, PrintExpression(exprs2[i]).c_str());
        // }
        exprNew = mergeFraction(exprs2);
        fprintf(stderr, "preprocessInit: after mergeFraction, exprNew = %s\n", PrintExpression(exprNew).c_str());
    }
    return exprNew;
}

std::unique_ptr<ExprAST> preprocess(const std::unique_ptr<ExprAST> &expr)
{
    std::unique_ptr<ExprAST> exprNew = preprocessInit(expr);

    if (isFraction(exprNew))
    {
        std::unique_ptr<ExprAST> numeratorTmp = getNumerator(exprNew);
        std::unique_ptr<ExprAST> denominatorTmp = getDenominator(exprNew);

        std::unique_ptr<ExprAST> numeratorNew = preprocess(numeratorTmp);
        std::unique_ptr<ExprAST> denominatorNew = preprocess(denominatorTmp);
        return combineFraction(numeratorNew, denominatorNew);
    }
    return exprNew;
}

std::vector<variableInfo> mergeVariables(std::vector<variableInfo> vec1, std::vector<variableInfo> vec2)
{
    std::vector<variableInfo> vec3;
    for (long unsigned int i = 0; i < vec1.size(); i++)
    {
        variableInfo &varTmp1 = vec1.at(i);
        variableInfo varTmp3 = varTmp1;

        for (long unsigned int j = 0; j < vec2.size(); j++)
        {
            variableInfo &varTmp2 = vec2.at(j);
            if (varTmp1.name == varTmp2.name)
            {
                varTmp3.degree = varTmp1.degree + varTmp2.degree;
                vec2.erase(vec2.begin() + j);
                break;
            }
        }
        vec3.push_back(varTmp3);
    }
    vec3.insert(vec3.end(), vec2.begin(), vec2.end());

    return vec3;
}

monoInfo mergeMonomial(const monoInfo &mono1, const monoInfo &mono2)
{
    monoInfo monoFinal;
    double &coefficient = monoFinal.coefficient;
    std::vector<funcInfo> &functions = monoFinal.functions;
    std::vector<variableInfo> &variables = monoFinal.variables;
    variableInfo variableTmp;

    coefficient = mono1.coefficient * mono2.coefficient;
    variables = mergeVariables(mono1.variables, mono2.variables);

    for(size_t i = 0; i < (mono1.functions).size(); ++i)
    {
        auto arg = (mono1.functions).at(i);
        functions.push_back(std::move(arg));
    }
    for(size_t i = 0; i < (mono2.functions).size(); ++i)
    {
        auto arg = (mono2.functions).at(i);
        functions.push_back(std::move(arg));
    }

    return monoFinal;
}

std::vector<monoInfo> extractInfo(const std::vector<std::unique_ptr<ExprAST>> &exprs);

monoInfo extractInfoKernel(const std::unique_ptr<ExprAST> &expr)
{
    // fprintf(stderr, "extractInfoKernel: at the begin: expr = %s\n", PrintExpression(expr).c_str());
    monoInfo monoFinal;
    double &coefficient = monoFinal.coefficient;
    std::vector<funcInfo> &functions = monoFinal.functions;
    std::vector<variableInfo> &variables = monoFinal.variables;
    funcInfo funcTmp;
    variableInfo variableTmp;

    std::string exprType = expr->type();
    std::vector<std::unique_ptr<ExprAST>> items;
    if (exprType == "Number")
    {
        // fprintf(stderr, "extractInfoKernel: number\n");
        NumberExprAST *numberPtr = dynamic_cast<NumberExprAST *>(expr.get());
        coefficient = numberPtr->getNumber();
        return monoFinal;
    }
    else if (exprType == "Variable")
    {
        // fprintf(stderr, "extractInfoKernel: variable\n");
        VariableExprAST *variablePtr = dynamic_cast<VariableExprAST *>(expr.get());
        variableTmp.name = variablePtr->getVariable();
        variableTmp.degree = 1;
        variables.push_back(variableTmp);
        return monoFinal;
    }
    else if (exprType == "Call")
    {
        // fprintf(stderr, "extractInfoKernel: variable\n");
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        funcTmp.callee = callPtr->getCallee();
        for(size_t i = 0; i < (callPtr->getArgs()).size(); ++i)
        {
            auto argAST = (callPtr->getArgs().at(i))->Clone();
            auto exprsTmp = extractItems(argAST);
            auto monosTmp = extractInfo(exprsTmp);
            polyInfo poly;
            for(auto monoTmp : monosTmp)
            {
                (poly.monos).push_back(monoTmp);
            }
            (funcTmp.args).push_back(poly);
        }
        functions.push_back(funcTmp);
        return monoFinal;
    }
    if (exprType != "Binary")
    {   // eg: poly := exp(x) or x or 2.2
        fprintf(stderr, "extractInfoKernel: expr is neither number, variable, call or binary\n");
        return monoFinal;
    }
    BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
    char op = binOpPtr->getOp();
    if (op == '*')
    {
        // fprintf(stderr, "extractInfoKernel: expr op is '*'\n");
        std::unique_ptr<ExprAST> &lhs = binOpPtr->getLHS();
        monoInfo monoTmp1 = extractInfoKernel(lhs);
        std::unique_ptr<ExprAST> &rhs = binOpPtr->getRHS();
        monoInfo monoTmp2 = extractInfoKernel(rhs);

        monoFinal = mergeMonomial(monoTmp1, monoTmp2);
    }
    else
    {
        fprintf(stderr, "\tERROR: extractInfoKernel: expr contains '%c'\n", op);
    }
    return monoFinal;
}

std::vector<monoInfo> extractInfo(const std::vector<std::unique_ptr<ExprAST>> &exprs)
{
    // fprintf(stderr, "extractInfo: start--------\n");
    std::vector<monoInfo> results;
    for (long unsigned int i = 0; i < exprs.size(); i++)
    {
        std::unique_ptr<ExprAST> exprTmp = exprs.at(i)->Clone();
        monoInfo monoTmp = extractInfoKernel(exprTmp);
        if(monoTmp.coefficient != 0)
        {
            std::vector<variableInfo> &variables = monoTmp.variables;
            std::sort(variables.begin(), variables.end());
            results.push_back(monoTmp);
        }
    }

    // print information of info
    // for (long unsigned int i = 0; i < results.size(); i++)
    // {
    //     fprintf(stderr, "The monoInfo No.%lu: \n", i);
    //     (results.at(i)).showInfo();
    //     fprintf(stderr, "\n");
    // }
    // fprintf(stderr, "extractInfo: end----------\n");
    return results;
}

std::vector<monoInfo> mergePolynomial(const std::vector<monoInfo> &info)
{
    // fprintf(stderr, "mergePolynomial: start--------\n");
    size_t size = info.size();
    bool *merged = new bool[size]{};
    size_t i = 0, j = 0;
    std::vector<monoInfo> results;
    while (i < info.size())
    {
        if(merged[i])
        {
            i++;
            continue;
        }
        monoInfo tmp = info.at(i);
        merged[i] = true;
        j = i + 1;
        while (j < info.size())
        {
            monoInfo tmp1 = info.at(j);
            if(tmp.hasCommonTerm(tmp1))
            {
                tmp.combine(tmp1);
                merged[j] = true;
            }
            j++;
        }
        results.push_back(tmp);
    }
    // TODO: check sort. Eg: b*d + a*b + b*b + a*a, output should be a*a + a*b + b*b + b*d, but is a*a + b*b + a*b + b*d;
    std::sort(results.begin(), results.end());

    // print information of info
    // for (size_t i = 0; i < results.size(); i++)
    // {
    //     fprintf(stderr, "The Monomial No.%lu: \n", i);
    //     (results.at(i)).showInfo();
    //     fprintf(stderr, "\n");
    // }
    delete []merged;
    // fprintf(stderr, "mergePolynomial: end----------\n");
    return results;
}

std::unique_ptr<ExprAST> geneFunctionAST(const funcInfo &func)
{
    std::string callee = func.callee;
    std::vector<std::unique_ptr<ExprAST>> argASTs;
    for(const polyInfo &arg : func.args)
    {
        std::unique_ptr<ExprAST> argAST = geneExprAST(arg.monos);
        argASTs.push_back(std::move(argAST));
    }
    return std::make_unique<CallExprAST>(callee, std::move(argASTs));
}

std::unique_ptr<ExprAST> geneMonomialAST(const monoInfo &monomial)
{
    const std::vector<variableInfo> &vars = monomial.variables;
    const std::vector<funcInfo> &funcs = monomial.functions;
    const polyInfo &poly = monomial.poly;
    if (vars.size() == 0 && funcs.size() == 0 && poly.monos.size() == 0) // number
    {
        return std::make_unique<NumberExprAST>(monomial.coefficient);
    }
    else if ((vars.size() == 1) && (vars.at(0).degree == 1) && (monomial.coefficient == 1) && (funcs.size() == 0) && poly.monos.size() == 0) // single variable
    {
        return std::make_unique<VariableExprAST>(vars.at(0).name);
    }
    else if (vars.size() == 0 && monomial.coefficient == 1 && funcs.size() == 1 && poly.monos.size() == 0) // single function
    {
        funcInfo func = funcs.at(0);
        return geneFunctionAST(func);
    }
    else // many variables or functions
    {
        // set monomial's coefficient
        std::unique_ptr<ExprAST> newExpr = nullptr;
        if(monomial.coefficient != 1)
        {
            newExpr = std::make_unique<NumberExprAST>(monomial.coefficient);
        }

        // set monomial's functions
        const std::vector<funcInfo> &functions = monomial.functions;
        for (const auto &func : functions)
        {
            std::unique_ptr<ExprAST> funcAST = geneFunctionAST(func);
            newExpr = mulExpr(std::move(newExpr), std::move(funcAST));
        }

        // set the addtional functions in the monomial.
        // NOTE: If there is poly, there is no func, and if there is func, there is no poly
        std::unique_ptr<ExprAST> tempMonos = nullptr;
        for (const auto &mono : poly.monos)
        {
            std::unique_ptr<ExprAST> tempMono = geneMonomialAST(mono);
            tempMonos = addExpr(std::move(tempMonos), std::move(tempMono));
        }
        if (tempMonos != nullptr)
        {
            newExpr = mulExpr(std::move(newExpr), std::move(tempMonos));
        }

        // set monomial's variables
        for (const auto &var : vars)
        {
            std::unique_ptr<ExprAST> elementAST = std::make_unique<VariableExprAST>(var.name);
            std::unique_ptr<ExprAST> varAST = nullptr;
            for (int j = 0; j < var.degree; j++)
            {
                varAST = mulExpr(std::move(varAST), std::move(elementAST->Clone()));
            }
            newExpr = mulExpr(std::move(newExpr), std::move(varAST));
        }

        return newExpr;
    }
}

std::unique_ptr<ExprAST> geneExprAST(const std::vector<monoInfo> &monos)
{
    // fprintf(stderr, "geneExprAST: start--------\n");
    if (monos.size() == 0)
    {
        fprintf(stderr, "ERROR: geneExprAST: input is NONE!\n");
    }
    
    std::unique_ptr<ExprAST> newExpr, tempAST;
    for (const auto &mono: monos)
    {
        tempAST = geneMonomialAST(mono);
        newExpr = addExpr(std::move(newExpr), std::move(tempAST));
    }
    // fprintf(stderr, "geneExprAST: end--------\n");
    return newExpr;
}

std::unique_ptr<ExprAST> geneExprASTYHR(std::vector<monoInfo> &info)
{
    fprintf(stderr, "geneExprAST: start--------\n");
    // info为monomial对象容器
    //表达式为数字单项式
    if (info.size() == 1 && info.at(0).variables.size() == 0)
    {
        std::unique_ptr<NumberExprAST> newExpr = std::make_unique<NumberExprAST>(NumberExprAST(info.at(0).coefficient));
        fprintf(stderr, "geneExprAST: end--------\n");
        return newExpr.get()->Clone();
    }
    //表达式为变量单项式
    if (info.size() == 1 && info.at(0).variables.size() == 1 && info.at(0).variables.at(0).degree == 1 && info.at(0).coefficient == 1)
    {
        std::unique_ptr<VariableExprAST> newExpr = std::make_unique<VariableExprAST>(VariableExprAST(info.at(0).variables.at(0).name));
        fprintf(stderr, "geneExprAST: end--------\n");
        return newExpr.get()->Clone();
    }

    std::unique_ptr<BinaryExprAST> newExpr = std::make_unique<BinaryExprAST>(BinaryExprAST('+', nullptr, nullptr));
    std::unique_ptr<BinaryExprAST> tempBinaryAST = nullptr;
    std::unique_ptr<BinaryExprAST> binaryASTLhs = nullptr;
    std::unique_ptr<ExprAST> ExprASTASTVar = nullptr;
    std::unique_ptr<ExprAST> tempBinaryASTLhs = nullptr;
    //先创建一�?临时的binaryAST,将�??一�?表达式作为左子树
    for (size_t i = 0; i < info.size(); i++)
    {
        if (i == 0)
        {
            //表达式为数字单项式
            if (info.at(i).variables.size() == 0)
            {
                tempBinaryASTLhs = std::move(std::make_unique<NumberExprAST>(NumberExprAST(info.at(i).coefficient)));
            }
            //表达式为变量单项式
            else if ((info.at(i).variables.size() == 1) && (info.at(i).coefficient == 1) && (info.at(i).variables.at(0).degree == 1))
            {
                tempBinaryASTLhs = std::move(std::make_unique<VariableExprAST>(VariableExprAST(info.at(i).variables.at(0).name)));
            }
            else
            {
                //该�?�器存放变量�?
                std::vector<std::string> varVec1;

                //将变量名存入容器
                for (size_t m = 0; m < info.at(i).variables.size(); m++)
                {
                    for (int n = 0; n < info.at(i).variables.at(m).degree; n++)
                    {
                        varVec1.push_back(info.at(i).variables.at(m).name);
                    }
                }

                //对�?�器排序
                std::sort(varVec1.begin(), varVec1.end());
                // std::cout << "begin to print vector" << std::endl;
                // for (int k = 0; k < varVec1.size(); k++)
                // {
                //     std::cout << varVec1.at(k) << std::endl;
                // }
                // std::cout << "print end" << std::endl;

                //系数不等于1
                if (info.at(i).coefficient != 1)
                {
                    // std::cout << "coefficient:" << info.at(i).coefficient << std::endl;
                    std::unique_ptr<NumberExprAST> numAST = std::make_unique<NumberExprAST>(NumberExprAST(info.at(i).coefficient));

                    binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(numAST), nullptr)));

                    for (size_t v = 0; v < varVec1.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec1.at(v)));
                        std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                        binaryASTLhs.get()->setRHS(varAST1);

                        if (v == varVec1.size() - 1)
                            break;
                        std::unique_ptr<ExprAST> tempBinaryAST3 = binaryASTLhs.get()->Clone();
                        binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                    if (info.size() == 1)
                    {
                        fprintf(stderr, "geneExprAST: end--------\n");
                        return binaryASTLhs->Clone();
                    }
                }
                else //系数等于1
                {
                    std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec1.at(0)));

                    binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(varAST), nullptr)));
                    for (size_t v = 1; v < varVec1.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec1.at(v)));
                        std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                        binaryASTLhs.get()->setRHS(varAST1);

                        if (v == varVec1.size() - 1)
                            break;
                        std::unique_ptr<ExprAST> tempBinaryAST3 = binaryASTLhs.get()->Clone();
                        binaryASTLhs = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                }
                tempBinaryASTLhs = binaryASTLhs->Clone();
            }
        }
        else
        {
            // std::cout << "here" << std::endl;

            //表达式为变量单项式
            if ((info.at(i).variables.size() == 1) && (info.at(i).variables.at(0).degree == 1) && i > 0 && info.at(i).coefficient == 1)
            {
                ExprASTASTVar = std::move(std::make_unique<VariableExprAST>(VariableExprAST(info.at(i).variables.at(0).name)));
            }
            else
            {
                //该�?�器存放变量�?
                std::vector<std::string> varVec2;
                //将变量名存入容器
                for (size_t m = 0; m < info.at(i).variables.size(); m++)
                {
                    for (int n = 0; n < info.at(i).variables.at(m).degree; n++)
                    {
                        varVec2.push_back(info.at(i).variables.at(m).name);
                    }
                }

                //对�?�器排序
                std::sort(varVec2.begin(), varVec2.end());

                // std::cout << "begin to print vector" << std::endl;
                // for (int k = 0; k < varVec2.size(); k++)
                // {
                //     std::cout << varVec2.at(k) << std::endl;
                // }

                //系数不等于1
                if (info.at(i).coefficient != 1)
                {
                    // std::cout << "coefficient:" << info.at(i).coefficient << std::endl;

                    std::unique_ptr<NumberExprAST> numAST = std::make_unique<NumberExprAST>(NumberExprAST(info.at(i).coefficient));

                    tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(numAST), nullptr)));

                    for (size_t v = 0; v < varVec2.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec2.at(v)));
                        std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                        tempBinaryAST.get()->setRHS(varAST1);

                        if (v == varVec2.size() - 1)
                            break;
                        std::unique_ptr<ExprAST> tempBinaryAST3 = tempBinaryAST.get()->Clone();
                        tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                }
                else //系数等于1
                {
                    std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec2.at(0)));

                    tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(varAST), nullptr)));
                    for (size_t v = 1; v < varVec2.size(); v++)
                    {
                        std::unique_ptr<VariableExprAST> varAST = std::make_unique<VariableExprAST>(VariableExprAST(varVec2.at(v)));
                        std::unique_ptr<ExprAST> varAST1 = varAST.get()->Clone();
                        tempBinaryAST.get()->setRHS(varAST1);

                        if (v == varVec2.size() - 1)
                            break;
                        std::unique_ptr<ExprAST> tempBinaryAST3 = tempBinaryAST.get()->Clone();
                        tempBinaryAST = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('*', std::move(tempBinaryAST3), nullptr)));
                    }
                }
            }
        }

        if (i == 0)
        {
            std::unique_ptr<ExprAST> tempBinaryAST1 = tempBinaryASTLhs.get()->Clone();
            newExpr->setLHS(tempBinaryAST1);
            if (info.size() == 1)
            {
                fprintf(stderr, "geneExprAST: end--------\n");
                return tempBinaryAST1->Clone();
            }
            continue;
        }
        else
        {
            std::unique_ptr<ExprAST> tempBinaryAST2 = nullptr;
            if ((info.at(i).variables.size() == 1) && (info.at(i).variables.at(0).degree == 1) && (info.at(i).coefficient == 1))
            {
                tempBinaryAST2 = std::move(ExprASTASTVar.get()->Clone());
            }
            else
            {
                tempBinaryAST2 = std::move(tempBinaryAST.get()->Clone());
            }
            newExpr->setRHS(tempBinaryAST2);
        }
        if (i != (info.size() - 1))
        {
            std::unique_ptr<ExprAST> newExpr1 = newExpr.get()->Clone();
            newExpr = std::move(std::make_unique<BinaryExprAST>(BinaryExprAST('+', std::move(newExpr1), nullptr)));
        }
    }
    fprintf(stderr, "geneExprAST: end--------\n");
    return newExpr->Clone();
}

// a kernel function within mathfuncRewrite
std::unique_ptr<ExprAST> dealWithCall(const std::unique_ptr<ExprAST> &expr)
{
    CallExprAST *callExpr1 = dynamic_cast<CallExprAST *>(expr.get());
    std::string callee = callExpr1->getCallee();
    std::unique_ptr<ExprAST> result;
    if(callee == "exp")
    {
        std::cout << "That is " << callee << std::endl;
        result = lex_x_Or_elx_x(expr);
    }
    else if(callee == "log")
    {   
        std::cout << "That is " << callee << std::endl;
        result = lex_x_Or_elx_x(expr);
        result = logTolog1p(result);
    }
    else if(callee == "sqrt")
    {
        std::cout << "That is " << callee << std::endl;
        result = sqrtTohypot(expr); // if success then will be core dumped
    }
    else
    {
        std::cout << "default: That is " << callee << std::endl;
        result = expr->Clone();
    }
    return result;
}

// TODO: improve
std::vector<std::unique_ptr<ExprAST>> mathfuncRewrite(const std::unique_ptr<ExprAST> &expr)
{
    fprintf(stderr, "\tmathfuncRewrite: start--------\n");
    
    if(expr == nullptr)
    {
        fprintf(stderr, "empty\n");
        return {};
    }
    std::unique_ptr<ExprAST> newexpr = expr->Clone();
    std::vector<std::unique_ptr<ExprAST>> exprsFinal;
    if(newexpr->type() == "Call")
    {
        newexpr = dealWithCall(newexpr);
    }
    else if(newexpr->type() == "Binary")
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(newexpr.get());
        char op = binOpPtr->getOp();
        if (op == '+')
        {
            newexpr = expToexpm1(newexpr);
        }
        while(op == '*')
        {
            std::unique_ptr<ExprAST> &rhs = binOpPtr->getRHS();
            // rhs
            BinaryExprAST *binOpRPtr = nullptr;
            if (rhs->type() == "Call")
            {
                std::unique_ptr<ExprAST> tmp = dealWithCall(rhs);
                binOpPtr->setRHS(tmp);
            }
            else if (rhs->type() == "Binary")
            {  // the usual case
                binOpRPtr = dynamic_cast<BinaryExprAST *>(rhs.get());
                char rhsOp = binOpRPtr->getOp();
                if (rhsOp == '+')
                {
                    std::unique_ptr<ExprAST> tmp = expToexpm1(rhs);
                    binOpPtr->setRHS(tmp);
                }
            }

            // lhs
            std::unique_ptr<ExprAST> &lhs = binOpPtr->getLHS();
            std::string lhsType = lhs->type();
            if(lhsType == "Binary")
            {
                binOpPtr = dynamic_cast<BinaryExprAST *>(lhs.get());
                op = binOpPtr->getOp();
                if(op == '+')
                {
                    std::unique_ptr<ExprAST> tmp = expToexpm1(lhs);
                    binOpPtr->setLHS(tmp);
                    break;
                }
            }
            else if(lhsType == "Call")
            {
                std::unique_ptr<ExprAST> tmp = dealWithCall(lhs);
                binOpPtr->setLHS(tmp);
                break;
            }
            else
            {
                break;
            }
        }
    }
    
    exprsFinal.push_back(std::move(newexpr));
    // std::vector<std::unique_ptr<ExprAST>> results;
    // exprsFinal.push_back(std::move(expr->Clone()));
    fprintf(stderr, "\tmathfuncRewrite: end--------\n");
    return exprsFinal;
}

// a kernel function within rewriteExpr
std::vector<size_t> getCombineOrders(const std::vector<size_t> widths, const size_t identifier)
{
    std::vector<size_t> orders;
    size_t order, start = 0;
    for(const auto &width : widths)
    {
        order = start + identifier % width;
        orders.push_back(order);
        order = identifier / width;
        start += width;
    }
    return orders;
}

// a kernel function within rewriteExpr
std::unique_ptr<ExprAST> getExprFromVariants(const std::vector<std::unique_ptr<ExprAST>> &variants, const std::vector<size_t> orders)
{
    std::unique_ptr<ExprAST> result = nullptr;
    for(const auto &order : orders)
    {
        result = addExpr(result, variants.at(order));
    }
    return result;
}

// a kernel function within rewriteExpr
std::vector<std::unique_ptr<ExprAST>> combineMonomial(const std::vector<std::unique_ptr<ExprAST>> &variants, const std::vector<size_t> widths)
{
    std::vector<std::unique_ptr<ExprAST>> results;
    size_t numOfCombinations = std::accumulate(std::begin(widths), std::end(widths), 1, std::multiplies<>());

    for(size_t i = 0; i < numOfCombinations; i++)
    {
        std::vector<size_t> orders = getCombineOrders(widths, i);
        std::unique_ptr<ExprAST> tmp = getExprFromVariants(variants, orders);
        results.push_back(std::move(tmp));
    }
    return results;
}

// TODO: check
std::vector<std::unique_ptr<ExprAST>> rewriteExpr(const std::vector<monoInfo> &monomials)
{
    fprintf(stderr, "rewriteExpr: start--------\n");
    std::vector<std::unique_ptr<ExprAST>> variants;
    std::vector<size_t> widths;

    for(const auto &monomial : monomials)
    {
        std::unique_ptr<ExprAST> expr = geneMonomialAST(monomial);
        std::vector<std::unique_ptr<ExprAST>> exprs = mathfuncRewrite(expr);
        variants.insert(variants.end(), std::make_move_iterator(exprs.begin()), std::make_move_iterator(exprs.end()));
        widths.push_back(exprs.size());
    }

    std::for_each(variants.begin(), variants.end(),
                [index = 0](const auto &variant) mutable { fprintf(stderr, "rewriteExpr: before middle, No.%d: %s\n", index++, PrintExpression(variant).c_str()); });
    std::vector<std::unique_ptr<ExprAST>> middles = combineMonomial(variants, widths);
    std::for_each(middles.begin(), middles.end(),
                [index = 0](const auto &variant) mutable { fprintf(stderr, "rewriteExpr: after middle, No.%d: %s\n", index++, PrintExpression(variant).c_str()); });
    std::vector<std::unique_ptr<ExprAST>> results;

    // for(const auto &middle : middles)
    // {
        // std::vector<std::unique_ptr<ExprAST>> tmps = createExpr(middle);
        // results.insert(results.end(), std::make_move_iterator(tmps.begin()), std::make_move_iterator(tmps.end()));
    // }
    // results = createExpr(expr);
    // results.push_back(std::move(expr->Clone()));
    fprintf(stderr, "rewriteExpr: end--------\n");
    return results;
}

// TODO: check
std::vector<std::unique_ptr<ExprAST>> rewriteExprWrapper(std::unique_ptr<ExprAST> &expr)
{
    fprintf(stderr, "rewriteExprWrapper: start: expr = %s\n", PrintExpression(expr).c_str());
    std::vector<std::unique_ptr<ExprAST>> items = extractItems(expr);
    std::vector<monoInfo> info = extractInfo(items);
    std::vector<monoInfo> infoNew = mergePolynomial(info);
    std::unique_ptr<ExprAST> exprNew = geneExprAST(infoNew);
    fprintf(stderr, "rewriteExprWrapper: after geneExprAST: exprNew = %s\n", PrintExpression(exprNew).c_str());
    exprNew = simplifyExpr(exprNew);
    return rewriteExpr(infoNew);
}

std::vector<std::unique_ptr<ExprAST>> createAll(std::vector<std::unique_ptr<ExprAST>> &numerators, std::vector<std::unique_ptr<ExprAST>> &denominators)
{
    std::unique_ptr<ExprAST> resultsTemp;
    std::vector<std::unique_ptr<ExprAST>> results;

    if(numerators.at(0) != nullptr && denominators.at(0) != nullptr)
    {
        for(long unsigned int i = 0; i < numerators.size(); i++)
        {
            for(long unsigned int j = 0; j < denominators.size(); j++)
            {
                resultsTemp = divExpr(numerators.at(i), denominators.at(j));
                results.push_back(std::move(resultsTemp));
            }
        }
        return results;
    }
    else
    {
        fprintf(stderr, "\tERROR: creatAll: the input include nullptr!\n");
        return {};
    }
}

// TODO: check
std::vector<std::unique_ptr<ExprAST>> exprAuto(std::unique_ptr<ExprAST> &expr)
{
    if (expr == nullptr)
    {
        fprintf(stderr, "exprAuto: ERROR: the input expr is nullptr!\n");
        return {};
    }
    fprintf(stderr, "exprAuto: step1: preprocess\n");
    fprintf(stderr, "exprAuto: expr = %s\n", PrintExpression(expr).c_str());
    std::unique_ptr<ExprAST> exprNew = preprocess(expr);
    fprintf(stderr, "exprAuto: exprNew = %s\n", PrintExpression(exprNew).c_str());

    std::vector<std::unique_ptr<ExprAST>> exprsFinal;
    fprintf(stderr, "exprAuto: step2: judge if exprNew is a fraction\n");
    if (isFraction(exprNew))
    {
        fprintf(stderr, "exprAuto: exprNew is a fraction, so perform step3 and step4\n");
        std::unique_ptr<ExprAST> numeratorTmp = getNumerator(exprNew);
        std::unique_ptr<ExprAST> denominatorTmp = getDenominator(exprNew);

        auto numeratorsFinal = rewriteExprWrapper(numeratorTmp);
        auto denominatorsFinal = rewriteExprWrapper(denominatorTmp);
        exprsFinal = createAll(numeratorsFinal, denominatorsFinal);
    }
    else
    {
        fprintf(stderr, "exprAuto: exprNew is not a fraction, so perform step4\n");
        exprsFinal = rewriteExprWrapper(exprNew);
    }

    fprintf(stderr, "exprAuto: exprsFinal size = %ld\n", exprsFinal.size());
    for (long unsigned int i = 0; i < exprsFinal.size(); i++)
    {
        fprintf(stderr, "exprAuto: No.%lu: %s\n", i, PrintExpression(exprsFinal[i]).c_str());
    }
    return exprsFinal;
}
