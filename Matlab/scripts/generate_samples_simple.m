% Carregar as posi??es do arquivo csv
% Plotar coordenadas separadamente para verificar o threshold
% Gerar as amostras com a melhor coord e threshold
function generate_samples_simple(cord, threshold, fileName)

fileID = fopen(['samples_', fileName, '.txt'],'w');

isNew = 0;

file_gesture = load([fileName, '.csv']);

for j=1:length(file_gesture(:,1))
    
    if(file_gesture(j, cord) < threshold)
        isNew = 0;
        continue;
    else if(file_gesture(j, cord) > threshold && isNew == 0)
            isNew = 1;
            fprintf(fileID, 'end\n');
            fprintf(fileID,'gesture %s hands 2\n', getNameGesture(fileName));
            fprintf(fileID,'%s %s %s %s %s %s\n', file_gesture(j, 1), ...
                                     file_gesture(j, 2), ...
                                     file_gesture(j, 3), ...
                                     file_gesture(j, 4), ...
                                     file_gesture(j, 5), ...
                                     file_gesture(j, 6));
        else
           fprintf(fileID,'%s %s %s %s %s %s\n', file_gesture(j, 1), ...
                                     file_gesture(j, 2), ...
                                     file_gesture(j, 3), ...
                                     file_gesture(j, 4), ...
                                     file_gesture(j, 5), ...
                                     file_gesture(j, 6));
        end
    end
end
end

% Return the name of the gesture
function name = getNameGesture(fileName)
    tokens = strsplit(fileName, '_');
    token = tokens(1,3);
    name = strcat('g',token{1});
end