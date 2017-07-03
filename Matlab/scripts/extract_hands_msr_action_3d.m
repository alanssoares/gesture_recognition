function extract_hands_msr_action_3d()

fileID = fopen('msr_action_3d.txt','w');
df = dir('*.txt');
total_gestures = numel(df);

for i=1:total_gestures
    
    file_gesture = load(df(i).name);
    k = 1;
    l = length(file_gesture(:,1));
    fprintf(fileID,'gesture %s hands 2\n', getNameGesture(df(i).name));
    
    for j=11:11:l
        
        if (j + 1 < l)
            %Left hand
            m(k, 1) = file_gesture(j, 1);
            m(k, 2) = file_gesture(j, 2);
            m(k, 3) = file_gesture(j, 3);
            %Right hand
            m(k, 4) = file_gesture(j + 1, 1);
            m(k, 5) = file_gesture(j + 1, 2);
            m(k, 6) = file_gesture(j + 1, 3);

            fprintf(fileID,'%s %s %s %s %s %s\n', m(k, 1), ...
                         m(k, 2), ...
                         m(k, 3), ...
                         m(k, 4), ...
                         m(k, 5), ...
                         m(k, 6));
                                 
            % new sample
            k = k + 1;
        end
    end
    
    fprintf(fileID, 'end\n');
    
    % initialize
    m = [];
end

% Return the name of the gesture
function name = getNameGesture(fileName)
    tokens = strsplit(fileName, '_');
    token = tokens(1,1);
    name = strcat('',token{1});%convert call to str and concat
end

end