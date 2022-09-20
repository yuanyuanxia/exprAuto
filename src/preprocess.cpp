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

#include "preprocess.hpp"

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
    if(expr->type() == "Call")
    {
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        std::vector<std::unique_ptr<ExprAST>> &args = callPtr->getArgs();
        std::vector<std::unique_ptr<ExprAST>> argsNew;
        
        for (auto const &arg : args)
        {
            auto tmp = minusRewrite(arg);
            argsNew.push_back(std::move(tmp));
        }
        return std::make_unique<CallExprAST>(callPtr->getCallee(), std::move(argsNew));
    }
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
    // std::unique_ptr<ExprAST> exprNew = std::move(expr->Clone());
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
