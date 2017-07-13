function read_gestures()

% Dataset cont?m na primeira coluna um id
% M?o esquerda come?a coluna 25
% M?o direita come?a na coluna 41
% https://www.microsoft.com/en-us/download/details.aspx?id=52283

fileID = fopen('msrc_12_raw.txt','w');

df = dir('*.csv');
total_gestures = numel(df);

for i=1:total_gestures
    
    file_gesture = load(df(i).name);
    fprintf(fileID,'gesture %s hands 2\n', getNameGesture(df(i).name));
    for j=1:length(file_gesture(:,1))
        fprintf(fileID,'%f %f %f %f %f %f\n', file_gesture(j, 26), ...
                 file_gesture(j, 27), ...
                 file_gesture(j, 28), ...
                 file_gesture(j, 42), ...
                 file_gesture(j, 43), ...
                 file_gesture(j, 44));
    end
    
    fprintf(fileID, 'end\n');
    
end

% Return the name of the gesture
function name = getNameGesture(fileName)
    tokens = strsplit(fileName, '_');
    token = tokens(1,3);
    name = strcat('',token{1});%convert call to str and concat
end

end
