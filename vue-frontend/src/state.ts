import { reactive, readonly } from 'vue';

// Define a reactive object
const state = reactive({
  uri: 'http://172.20.10.5:80'
});

// Export the read-only version of the object
export default readonly(state);
