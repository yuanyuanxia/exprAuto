#include <algorithm>
#include <queue>
#include <numeric>

#include "basic.hpp"
#include "monoFracInfo.hpp"
#include "expandAST.hpp"
#include "exprAuto.hpp"
#include "polyRewrite.hpp"
#include "mathfuncRewrite.hpp"
#include "simplifyExpr.hpp"
#include "preprocess.hpp"

using std::string;
using std::vector;
using std::cout;
using std::endl;

ast_ptr combineFraction(const ast_ptr &numerator, const ast_ptr &denominator)
{
    // init
    ast_ptr numeratorNew = nullptr;
    ast_ptr denominatorNew = nullptr;
    ast_ptr numeratorFinal = nullptr;
    ast_ptr denominatorFinal = nullptr;
    // numerator
    if (isFraction(numerator))
    {
        const ast_ptr numeratorTmp = getNumerator(numerator);
        const ast_ptr denominatorTmp = getDenominator(numerator);

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
        const ast_ptr numeratorTmp = getNumerator(denominatorNew);
        const ast_ptr denominatorTmp = getDenominator(denominatorNew);

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

vector<ast_ptr> extractItems(const ast_ptr &expr)
{
    // fprintf(stderr, "extractItems: start--------\n");
    if (expr == nullptr)
    {
        fprintf(stderr, "\tERROR: extractItems: the input expr is nullptr!\n");
        return {};
    }
    vector<ast_ptr> items;
    if (expr->type() != "Binary")
    {
        ast_ptr exprTmp = expr->Clone();
        items.push_back(std::move(exprTmp));
    }
    else
    {
        BinaryExprAST *binOpPtr = dynamic_cast<BinaryExprAST *>(expr.get());
        char op = binOpPtr->getOp();

        if (op != '+')
        {
            ast_ptr exprTmp = expr->Clone();
            items.push_back(std::move(exprTmp));
        }
        else
        {
            ast_ptr &lhs = binOpPtr->getLHS();
            auto items1 = extractItems(lhs);
            ast_ptr &rhs = binOpPtr->getRHS();
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

ast_ptr moveDivKernel(const ast_ptr &expr)
{                                          // level Traversal
    std::queue<BinaryExprAST *> nodeQueue; // nodeQueue存储层序遍历的节点
    std::queue<char> sideQueue;            // sideQueue�?能存�?'L'�?'R',表示对应节点在根节点的左边还�?右边
    ast_ptr in = expr->Clone();
    const string exprType = expr->type();
    if (exprType == "Binary")
    {
        BinaryExprAST *root = dynamic_cast<BinaryExprAST *>(in.get());
        ast_ptr &rootLhs = root->getLHS();
        ast_ptr &rootRhs = root->getRHS();

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
                                    ast_ptr resultRight = nodeLHSPtr->getRHS()->Clone(); //'/'右子树resultRight成为result的右子树
                                    ast_ptr tmp = nodeLHSPtr->getLHS()->Clone();         //'/'左子树根节点将成为替�?'/'的节�?

                                    node->setLHS(tmp);                                   //改变node的左子树
                                    ast_ptr resultLeft = root->Clone(); // node左子树变化后，root将成为result左子�?

                                    return makePtr<BinaryExprAST>('/', std::move(resultLeft), std::move(resultRight));
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
                                    ast_ptr resultRight = nodeRHSPtr->getRHS()->Clone();
                                    ast_ptr tmp = nodeRHSPtr->getLHS()->Clone();

                                    node->setRHS(tmp); //改变node的右子树
                                    ast_ptr resultLeft = root->Clone();

                                    return makePtr<BinaryExprAST>('/', std::move(resultLeft), std::move(resultRight));
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

vector<ast_ptr> moveDiv(const vector<ast_ptr> &exprs)
{
    // fprintf(stderr, "moveDiv: start--------\n");
    vector<ast_ptr> items;
    ast_ptr moveBefore, moveAfter;
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

ast_ptr mergeFraction(const vector<ast_ptr> &exprs)
{
    //把分子和分母分开
    vector<ast_ptr> numerators;
    vector<ast_ptr> denominators;
    unsigned int fractionCount = 0;
    for (long unsigned int i = 0; i < exprs.size(); i++)
    {
        ast_ptr exprTmp = exprs.at(i)->Clone();
        ast_ptr numeratorTmp = nullptr;
        ast_ptr denominatorTmp = nullptr;
        if (isFraction(exprTmp))
        {
            numeratorTmp = getNumerator(exprTmp);
            denominatorTmp = getDenominator(exprTmp);
            fractionCount++;
        }
        else
        {
            numeratorTmp = std::move(exprTmp);
            denominatorTmp = makePtr<NumberExprAST>(1);
        }
        numerators.push_back(std::move(numeratorTmp));
        denominators.push_back(std::move(denominatorTmp));
    }
    //处理分子，进行循环合并
    vector<ast_ptr> numeratorcom;  //建立合并之后的分子数组
    ast_ptr tmpOne = makePtr<NumberExprAST>(1.0);
    for (long unsigned int i = 0; i < exprs.size(); i++) //写在循环里，每循环一次重置一次
    {
        ast_ptr exprTmp_i = numerators.at(i)->Clone();
        ast_ptr numeratorTmp = nullptr;
        numeratorTmp = std::move(exprTmp_i);
        for (long unsigned int j = 0; j < exprs.size(); j++)
        {
            if (i != j) //乘以除了原本分母的分母
            {
                ast_ptr exprTmp_j = denominators.at(j)->Clone();
                if(isEqual(exprTmp_j, tmpOne))
                {
                    numeratorTmp = std::move(numeratorTmp->Clone());
                }
                else
                {
                    numeratorTmp = mulExpr(numeratorTmp, exprTmp_j);
                }
            }
        }
        numeratorcom.push_back(std::move(numeratorTmp));
    }
    //合并分子
    ast_ptr exprTmp = numeratorcom.at(0)->Clone();
    ast_ptr result = std::move(exprTmp);
    for (long unsigned int i = 1; i < exprs.size(); i++)
    {
        exprTmp = numeratorcom.at(i)->Clone();
        result = addExpr(result, exprTmp);
    }
    //处理分母
    if (fractionCount > 0)
    {
        ast_ptr denominatorTmp = denominators.at(0)->Clone();
        for (long unsigned int i = 1; i < exprs.size(); i++) //循环把各自的分母变成公分母
        {
            exprTmp = denominators.at(i)->Clone();
            denominatorTmp = mulExpr(denominatorTmp, exprTmp);
        }
        result = divExpr(result, denominatorTmp);
    }
    return result;
}

ast_ptr minusRewrite(const ast_ptr &expr)
{
    if(expr->type() == "Call")
    {
        CallExprAST *callPtr = dynamic_cast<CallExprAST *>(expr.get());
        vector<ast_ptr> &args = callPtr->getArgs();
        vector<ast_ptr> argsNew;
        
        for (auto const &arg : args)
        {
            auto tmp = minusRewrite(arg);
            argsNew.push_back(std::move(tmp));
        }
        return makePtr<CallExprAST>(callPtr->getCallee(), std::move(argsNew));
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
        ast_ptr minusOne = makePtr<NumberExprAST>(-1);
        rhsNew = createBinaryExpr(minusOne, rhsNew, '*');
        op = '+';
    }
    return createBinaryExpr(lhsNew, rhsNew, op);
}

ast_ptr preprocessInit(const ast_ptr &expr)
{
    static size_t callCount = 0;
    callCount++;
    callLevel++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "preprocessInit: ";

    ast_ptr exprNew = minusRewrite(expr);
    // ast_ptr exprNew = std::move(expr->Clone());
    // fprintf(stderr, "preprocessInit: after minusRewrite, exprNew = %s\n", PrintExpression(exprNew).c_str());
    if (isFraction(expr))
    {
        exprNew = exprNew->Clone();
    }
    else
    {
        // fprintf(stderr, "preprocessInit: before expandExprWrapper, exprNew = %s\n", PrintExpression(exprNew).c_str());
        ast_ptr exprTmp = expandExprWrapper(exprNew);
        // fprintf(stderr, "preprocessInit: after expandExprWrapper, exprTmp = %s\n", PrintExpression(exprTmp).c_str());
        vector<ast_ptr> exprs1 = extractItems(exprTmp);
        // fprintf(stderr, "\tpreprocessInit: after extractItems: exprs1 size = %ld\n", exprs1.size());
        // for (size_t i = 0; i < exprs1.size(); i++)
        // {
            // fprintf(stderr, "\tpreprocessInit: after extractItems: No.%lu: %s\n", i, PrintExpression(exprs1[i]).c_str());
        // }
        vector<ast_ptr> exprs2 = moveDiv(exprs1);
        // fprintf(stderr, "\tpreprocessInit: after moveDiv: exprs2 size = %ld\n", exprs2.size());
        // for (size_t i = 0; i < exprs2.size(); i++)
        // {
        //     fprintf(stderr, "\tpreprocessInit: after moveDiv: No.%lu: %s\n", i, PrintExpression(exprs2[i]).c_str());
        // }
        exprNew = mergeFraction(exprs2);
        printExpr(exprNew, prompt + "at the last, after mergeFraction, exprNew = ");
    }
    callCount--;
    callLevel--;
    return exprNew;
}

ast_ptr preprocess(const ast_ptr &expr)
{
    static size_t callCount = 0;
    callCount++;
    string prompt(callLevel * promtTimes, callLevelChar);
    prompt.append(callCount, callCountChar);
    prompt += "preprocess: ";
    cout << prompt << "start--------" <<endl;
    ast_ptr exprNew = preprocessInit(expr);

    if (isFraction(exprNew))
    {
        ast_ptr numeratorTmp = getNumerator(exprNew);
        ast_ptr denominatorTmp = getDenominator(exprNew);

        ast_ptr numeratorNew = preprocess(numeratorTmp);
        ast_ptr denominatorNew = preprocess(denominatorTmp);
        return combineFraction(numeratorNew, denominatorNew);
    }
    cout << prompt << "end--------" <<endl;
    callCount--;
    return exprNew;
}
