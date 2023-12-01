<template>
    <div class="flex flex-col w-full py-10">
        <image-canvas :capture="capture"></image-canvas>
        <button class="btn btn-primary" @click="takePicture">Take photo</button>
    </div>
</template>
  
<script lang="ts">
import ImageCanvas from './ImageCanvas.vue'; // Adjust the path as necessary

export default {
    components: {
        ImageCanvas
    },
    data() {
        return {
            capture: false,
        }
    },
    methods: {
        takePicture() {
            this.capture = true;
            setTimeout(() => { this.capture = false; }, 500);
        }
    }
};
</script>
  