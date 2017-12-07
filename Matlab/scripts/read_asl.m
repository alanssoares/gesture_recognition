function read_asl(folder)
fileID = fopen(folder,'w');

df = dir('*.sign')
total_gestures = numel(df);

for i=1:total_gestures
    
    file_gesture = load(df(i).name);
    fprintf(fileID,'gesture %s hands 1\n', getNameGesture(df(i).name));
    for j=1:length(file_gesture(:,1))
        fprintf(fileID,'0.0 0.0 0.0 %f %f %f\n', ...
                 file_gesture(j, 1), ...
                 file_gesture(j, 2), ...
                 file_gesture(j, 3));
    end
    
    fprintf(fileID, 'end\n');
    
end

% Return the name of the gesture
function name = getNameGesture(fileName)
    tokens = strsplit(fileName, '.');
    token = tokens(1,1);
    name = strcat('', token{1});
    name = name(1:end-1);
end
end