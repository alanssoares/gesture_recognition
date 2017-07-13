function extract_hands_msr_action_3d()

fileID = fopen('msr_action_3d.txt','w');
df = dir('*_skeleton3D.txt');
total_gestures = numel(df);

for i=1:total_gestures
    
    file_gesture = load(df(i).name,'-ascii');
    
    l = length(file_gesture(:,1));
    fprintf(fileID,'gesture %s hands 2\n', getNameGesture(df(i).name));
    
    j = 12;
    
    while(j < l)
        
        if (j + 1 < l)
        fprintf(fileID,'%f %f %f %f %f %f\n', file_gesture(j + 1, 1), ...
                     file_gesture(j + 1, 2), ...
                     file_gesture(j + 1, 3), ...
                     file_gesture(j, 1), ...
                     file_gesture(j, 2), ...
                     file_gesture(j, 3));
        j = j + 20;
        end
        if (j + 1 >= l)
            break;
        end;
    end
    
    fprintf(fileID, 'end\n');

end

% Return the name of the gesture
function name = getNameGesture(fileName)
    tokens = strsplit(fileName, '_');
    token = tokens(1,1);
    name = strcat('',token{1});%convert call to str and concat
end

end