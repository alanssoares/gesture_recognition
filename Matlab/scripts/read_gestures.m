function read_gestures()

df = dir('*.csv');
total_gestures = numel(df);

for i=1:total_gestures
    
    file_gesture = load(df(i).name);
    
    for j=1:length(file_gesture(:,1))
        %Left hand
        m(j, 1) = file_gesture(j, 26);
        m(j, 2) = file_gesture(j, 27);
        m(j, 3) = file_gesture(j, 28);
        %Right hand
        m(j, 4) = file_gesture(j, 42);
        m(j, 5) = file_gesture(j, 43);
        m(j, 6) = file_gesture(j, 44);
    end
    
    % save the file
    csvwrite(sprintf('gestures_hand/%s', df(i).name), m);
    
    % initialize
    m = [];
end

end
