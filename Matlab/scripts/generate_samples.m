function generate_samples()

df = dir('*.csv');
fileID = fopen('samples_full.txt','w');

for i=1:numel(df)
    % load the file
    file_gesture = load(df(i).name);
    % header gesture
    fprintf(fileID,'gesture %s hands 2\n', getNameGesture(df(i).name));
    % positions of the hands
    for j=1:length(file_gesture(:,1))
        if(file_gesture(j, 1) == 0 && file_gesture(j, 2) == 0 && ...
            file_gesture(j, 3) == 0 && file_gesture(j, 4) == 0 && ...
            file_gesture(j, 5) == 0 && file_gesture(j, 6) == 0)
            continue;
        else
            fprintf(fileID,'%s %s %s %s %s %s\n', file_gesture(j, 1), ...
                                     file_gesture(j, 2), ...
                                     file_gesture(j, 3), ...
                                     file_gesture(j, 4), ...
                                     file_gesture(j, 5), ...
                                     file_gesture(j, 6));
        end
    end
    fprintf(fileID, 'end\n');   
end

end

% Return the name of the gesture
function name = getNameGesture(fileName)
    tokens = strsplit(fileName, '_');
    token = tokens(1,3);
    name = strcat('g',token{1});%convert call to str and concat
end