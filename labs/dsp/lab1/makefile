
roll_compile:
	g++ -o roll main.cpp
	
roll_run_encode:
	./roll e abcd output
	
roll_run_decode:
	./roll d output abcd_output
	
roll_test: roll_compile roll_run_encode roll_run_decode
	diff abcd abcd_decoded

samp_compile:
	g++ -o sample ./base/arcode.c

samp_run_encode:
	./sample e abcd output

samp_run_decode:
	./sample d output abcd_decoded

samp_test: samp_compile samp_run_encode samp_run_decode 
	diff abcd abcd_decoded


####################

compile:
	g++ -o arcode arcode.cpp
	
run_encode:
	./arcode e abcd output 

run_decode:
	./arcode d output abcd_output 
	
test: compile run_encode run_decode
	diff abcd abcd_decoded
	
	
#######################
F_ADAPTIVE:=-a
F_WITH_MEMORY:=-m

c_compile:
	g++ -o main main.cpp arcode.cpp
	
c_run_encode:
	./main -e -i abcd -o output $(F_ADAPTIVE) $(F_WITH_MEMORY)

c_run_decode:
	./main -d -i output -o abcd_output $(F_ADAPTIVE) $(F_WITH_MEMORY) 
	
c_test: c_compile c_run_encode c_run_decode
	diff abcd abcd_decoded
	
c_run_encode_image:
	./main -e -i image.tif -o image_encoded $(F_ADAPTIVE) $(F_WITH_MEMORY)

c_run_decode_image:
	./main -d -i image_encoded -o image_encoded.tif $(F_ADAPTIVE) $(F_WITH_MEMORY)
	
c_test_image: c_compile c_run_encode_image c_run_decode_image
	diff image.tif image_encoded.tif
