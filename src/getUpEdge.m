tic;
sampleFiles = strcat(outputFilesPath, sampleFileNameKernel);
namelist = dir(sampleFiles);
fileNum = length(namelist);
regexTemplate = '(?<=sample_).*?(?=.txt)';

for i = 1 : fileNum
    fileNameTmp = strcat(outputFilesPath, namelist(i).name);
    sampleData = importdata(fileNameTmp);

    strTmp = regexp(fileNameTmp, regexTemplate, 'match');
    sampleDataNameTmp = strTmp{1,1};
    upEdgeFileName = strcat(outputFilesPath, "upEdge_", sampleDataNameTmp, ".txt");

    lenTmp = length(sampleData);
    maxTmp = max(sampleData(:,2));
    maxIndex = find(sampleData(:,2)>=maxTmp);
    meanTmp = mean(sampleData(:,2));
    upperTmp = (maxTmp + meanTmp) / 10;
    domainTmp = 0.01;
    leftTmp = int64(max(1, maxIndex - lenTmp * domainTmp * 0.5));
    rightTmp = int64(min(lenTmp, maxIndex + lenTmp * domainTmp * 0.5));

    for j = 1:1 % 右端点需要更正为maxIndex的长度
        leftTmp(j) = int64(max(1, maxIndex(j) - lenTmp * domainTmp * 0.5));
        rightTmp(j) = int64(min(lenTmp, maxIndex(j) + lenTmp * domainTmp * 0.5));
        localTmp = find(sampleData(leftTmp(j):rightTmp(j),2) > upperTmp);
        wholeTmp = find(sampleData(:,2) > upperTmp);
        rateTmp = length(localTmp) / length(wholeTmp);
        edgeIndex = boundary(sampleData(:,1), sampleData(:,2), 1.0);
        edge = [edgeIndex, sampleData(edgeIndex,1), sampleData(edgeIndex,2)];
        save(upEdgeFileName, 'edge', '-ascii', '-double'); % the 2nd parameter mean the data to save to file
        % simple way
        % upEdgeIndex = edge(:,3)>=2;
        % upEdge = edge(upEdgeIndex,:);

        % upEdge: tradional way
        edge = flipud(edge); % 水平轴上下翻转数组
        if isempty(edge)
            fprintf("\tedge is empty!\n");
            break;
        end
        upEdgeStart = 1;
        upEdgeEnd = length(edge);
        edgeTmp = edge;
        thresholdTrash = 5;
        minEdgeInput = min(edgeTmp(:,2)); % 记录的是误差对应的输入
        maxEdgeInput = max(edgeTmp(:,2)); % 记录的是误差对应的输入
        
        threshold = 2;
        edgeTmp(edge(:,3)<threshold, 3) = threshold; % 把edge中误差小于阈值的置为阈值
        % ready for upEdge's head
        trash = 0;
        for k = 1:length(edgeTmp)
            if(edgeTmp(k,2) == minEdgeInput) % 比较的是误差对应的输入
                upEdgeStart = max(1,k);
                break
            end
            if(edgeTmp(k,end) ~= threshold) % 比较的是误差
                trash = trash + 1;
                if(trash > thresholdTrash)
                    upEdgeStart = max(1,k-1-thresholdTrash);
                    break
                end
            else
                trash = 0;
            end
        end
        % ready for upEdge's tail
        trash = 0;
        for k = length(edgeTmp):-1:1
            if(edgeTmp(k,2) == maxEdgeInput)
                upEdgeEnd = min(k, length(edgeTmp));
                break
            end
            if(edgeTmp(k,end) ~= threshold)
                trash = trash + 1;
                if(trash > thresholdTrash)
                    upEdgeEnd = min(k+1+thresholdTrash, length(edgeTmp));
                    break
                end
            else
                trash = 0;
            end
        end
        % after getting head & tail, we get upEdge
        upEdge = edge(upEdgeStart:upEdgeEnd,:); % upEdge是保留了上半段轮廓线中小于阈值的部分
        
        if rateTmp > 0.5 % 做判断是因为rateTmp大于0.5即说明其图形unstable，出现了畸变
            % fprintf("%s is unstable, rate = %g, maxError = %g\n", sampleFileName, rateTmp, maxTmp);
            % draw pic and save pic    
            % figure(1);
            % scatter(sampleData(:,1), sampleData(:,2), '.');
            % hold on; plot(edge(:,2), edge(:,3), '-r', 'LineWidth', 1); hold off;
            % set(gca, 'Fontname', 'Times newman', 'Fontsize', 55);
            % nameTmp = strcat("unstable_scatterEdge_", sampleFileName);
            % % title(nameTmp, 'Interpreter', 'none');
            % % grid on;
            % img =gcf; print(img, '-djpeg', '-r100', nameTmp);
            
            % update boundary error to [2, 10]。这一段其实可以放到前边去
            radio = (maxTmp - 2) / 8;
            indexTmp = sampleData(:,2) >= 2;
            sampleData(indexTmp, 2) = (sampleData(indexTmp,2) - 2) / radio + 2;
            maxTmp = max(sampleData(indexTmp,2));
            edgeIndex = boundary(sampleData(:,1), sampleData(:,2), 1.0);
            edge = [edgeIndex, sampleData(edgeIndex,1), sampleData(edgeIndex,2)];

            % upEdge: tradional way
            edge = flipud(edge);
            if isempty(edge)
                fprintf("\tedge is empty!\n");
                break;
            end
            upEdgeStart = 1;
            upEdgeEnd = length(edge);
            edgeTmp = edge;
            thresholdTrash = 5;
            minEdgeInput = min(edgeTmp(:,2));
            maxEdgeInput = max(edgeTmp(:,2));
            
            threshold = 2;
            edgeTmp(edge(:,3)<threshold, 3) = threshold;
            % head
            trash = 0;
            for k = 1:length(edgeTmp)
                if(edgeTmp(k,2) == minEdgeInput)
                    upEdgeStart = max(1,k);
                    break
                end
                if(edgeTmp(k,end) ~= threshold)
                    trash = trash + 1;
                    if(trash > thresholdTrash)
                        upEdgeStart = max(1,k-1-thresholdTrash);
                        break
                    end
                else
                    trash = 0;
                end
            end
            % tail
            trash = 0;
            for k = length(edgeTmp):-1:1
                if(edgeTmp(k,2) == maxEdgeInput)
                    upEdgeEnd = min(k, length(edgeTmp));
                    break
                end
                if(edgeTmp(k,end) ~= threshold)
                    trash = trash + 1;
                    if(trash > thresholdTrash)
                        upEdgeEnd = min(k+1+thresholdTrash, length(edgeTmp));
                        break
                    end
                else
                    trash = 0;
                end
            end
            % after head & tail
            upEdge = edge(upEdgeStart:upEdgeEnd,:); % upEdge是保留了上半段轮廓线中小于阈值的部分

            % draw pic and save pic
            % figure(1);
            % scatter(sampleData(:,1), sampleData(:,2), '.');
            % hold on; plot(upEdge(:,2), upEdge(:,3), '-r', 'LineWidth', 1); hold off;
            % set(gca, 'Fontname', 'Times newman', 'Fontsize', 55);
            % nameTmp = strcat("unstable_1_scatterEdge_", sampleFileName);
            % % title(nameTmp, 'Interpreter', 'none');
            % % grid on;
            % img =gcf; print(img, '-djpeg', '-r100', nameTmp);
        else % do nothing, just for debug
            % fprintf("%s is stable, rate = %g, maxError = %g\n", sampleFileName, rateTmp, maxTmp);
            % draw pic and save pic
            % figure(1);
            % scatter(sampleData(:,1), sampleData(:,2), '.');
            % hold on; plot(upEdge(:,2), upEdge(:,3), '-r', 'LineWidth', 1); hold off;
            % set(gca, 'Fontname', 'Times newman', 'Fontsize', 55);
            % nameTmp = strcat("stable_scatterEdge_", sampleFileName);
            % % title(nameTmp, 'Interpreter', 'none');
            % % grid on;
            % img =gcf; print(img, '-djpeg', '-r100', nameTmp);
        end

        % save upEdge data to file whose format like upEdge_Bspline3.txt
        if(~isempty(edge)) 
            % fprintf("\tupEdgeFileName: %s with %d lines\n", upEdgeFileName, length(upEdge));
            save(upEdgeFileName, 'upEdge', '-ascii', '-double'); % the 2nd parameter mean the data to save to file
        else
            fprintf("\tedge is empty\n");
        end

        % if(length(maxIndex) > 1)
        %     fprintf("\tleftTmp(%d) = %d, rightTmp(%d) = %d\n", j, leftTmp(j), j, rightTmp(j));
        % end
    end 
    if maxTmp > 2
        indexTmp = sampleData(:,2) >= 2;
        meanTmp = mean(sampleData(indexTmp, 2));
    else
        meanTmp = mean(sampleData(:, 2));
    end
end
all_time = toc;
fprintf("%f\n", all_time);
% compute the threshold for deviding intervals
% thresholdTmp = (maxTmp + meanTmp) / 2;
% fprintf("%s: max = %g, average = %g, threshold = %d\n", sampleFileName, maxTmp, meanTmp, thresholdTmp);

% 去除误差小于0.5的点，然后绘图。感觉区别不大。下面这段代码就是示例代码，仅仅是绘图，并没有保存数据或图片
% indexTmp = sampleData(:,2)>0.5;
% sampleDataTmp1 = (sampleDataTmp(indexTmp,:));
% edgeIndex = boundary(sampleDataTmp1(:,1), sampleDataTmp1(:,2), 1.0);
% edge = [edgeIndex, sampleDataTmp1(edgeIndex,1), sampleDataTmp1(edgeIndex,2)];
% figure(2);
% scatter(sampleDataTmp1(:,1), sampleDataTmp1(:,2), '.');
% hold on; plot(edge(:,2), edge(:,3), '-r', 'LineWidth', 1); hold off;
