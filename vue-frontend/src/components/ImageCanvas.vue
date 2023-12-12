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
            <h2 id="result-text" class="card-title">{{ response }}</h2>
        </div>
    </div>
</template>

<script lang="ts">
import state from '../state'

export default {
    computed: {
        response() {
            console.log(this.resultText);
            return this.resultText ? this.makeResponse(this.resultText) : 'No photo has been captured yet.';
        }
    },
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
        makeResponse(result: string) {
            const numbers: number[] = JSON.parse(result);

            const max = Math.max(...numbers);
            const idx = numbers.indexOf(max);
            const formatted_max = Math.round(max * 100) / 100;

            return 'The letter is: ' + String.fromCharCode(idx + 65) + ' (' + formatted_max + ')';
        },

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
                    const rawArr = new Uint8Array(arrBuffer);
                    const clampedArr = new Uint8ClampedArray(rawArr.byteLength * 4);

                    for (let i = 0; i < rawArr.byteLength; i++) {
                        clampedArr[i * 4] = rawArr[i];
                        clampedArr[i * 4 + 1] = rawArr[i];
                        clampedArr[i * 4 + 2] = rawArr[i];
                        clampedArr[i * 4 + 3] = 255;
                    }

                    const imageData = new ImageData(clampedArr, 320, 240);
                    const canvas = document.getElementById("canvas") as HTMLCanvasElement;
                    const ctx = canvas.getContext("2d") as CanvasRenderingContext2D;

                    ctx.putImageData(imageData, 0, 0);

                });
        }
    },
};
</script>
