# TensorFlow Lite: load model file and predict something

import numpy as np
import tflite_runtime.interpreter as tflite

# Load TFLite model and allocate tensors.
interpreter = tflite.Interpreter(model_path="/tmp/R-my_model.tflite")
#interpreter = tflite.Interpreter(model_path="/tmp/C-my_model.tflite")
interpreter.allocate_tensors()

# Get input and output tensors.
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

# Use model with input data.
input_shape = input_details[0]['shape']
input_data = np.array([[-5.6]], dtype=np.float32)  # R-my_model.tflite 
#input_data = np.array([[5.8,-3.5]], dtype=np.float32)  # C-my_model.tflite
interpreter.set_tensor(input_details[0]['index'], input_data)  

interpreter.invoke()

# The function 'get_tensor()' returns a copy of the tensor data.
# Use 'tensor()' in order to get a pointer to the tensor.
output_data = interpreter.get_tensor(output_details[0]['index'])
#print(np.round(output_data, 1))
print(np.round(output_data, 2))