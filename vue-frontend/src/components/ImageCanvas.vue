<template>
    <div class="card flex items-center justify-center text-center bg-base-100 shadow-xl max-w-screen-lg max-h-screen p-10">
        <div v-if="imageUrl != ''">
            <canvas id="canvas" class="max-w-[800px] max-h-[600px]"></canvas>
        </div>
        <div v-else class="flex items-center justify-center m-auto">
            <h1>
                No photo has been captured yet.
            </h1>
        </div>

        <div class="card-body">
            <h2 id="result-text" class="card-title">{{ resultText }}</h2>
        </div>
    </div>
</template>

<script lang="ts">
import state from '../state'

export default {
    props: {
        capture: Boolean
    },
    watch: {
        capture(newValue) {
            if (newValue) {
                this.captureImage();
            }
        }
    },
    data() {
        return {
            imageUrl: '',
            resultText: '',
        };
    },
    methods: {
        captureImage() {
            console.log("Capturing image...");
            fetch(state.uri + '/capture', { method: 'GET' })
                .then(response => response.text())
                .then(result => {
                    this.resultText = result;
                    this.imageUrl = "set"
                    return fetch(state.uri + '/saved-photo', { headers: { 'Content-Type': 'application/octet-stream' } });
                })
                .then(response => response.arrayBuffer())
                .then(arrBuffer => {
                    console.log(arrBuffer);
                    const rawArr = new Uint8Array(arrBuffer);
                    console.log(rawArr);
                    const clampedArr = new Uint8ClampedArray(rawArr.byteLength * 4);

                    for (let i = 0; i < rawArr.byteLength; i++) {
                        clampedArr[i * 4] = rawArr[i];
                        clampedArr[i * 4 + 1] = rawArr[i];
                        clampedArr[i * 4 + 2] = rawArr[i];
                        clampedArr[i * 4 + 3] = 255;
                    }

                    console.log(clampedArr);

                    const imageData = new ImageData(clampedArr, 320, 240);
                    console.log(imageData);
                    const canvas = document.getElementById("canvas") as HTMLCanvasElement;
                    const ctx = canvas.getContext("2d") as CanvasRenderingContext2D;

                    ctx.putImageData(imageData, 0, 0);

                });
        }
    },
};
</script>
