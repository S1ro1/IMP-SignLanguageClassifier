<template>
  <div class="navbar bg-base-100 w-full fixed top-0 left-0">
    <div class="navbar-start">
      <div class="dropdown w-1/2">
        <div tabindex="0" role="button" class="btn btn-lg btn-ghost btn-circle">
          <img src="../assets/logo.svg" />
        </div>
        <div class="menu menu-sm dropdown-content mt-3 z-[1] p-2 shadow bg-base-100 rounded-box w-full">
          <h1 class="font-bold text-2xl">
            Reconfigure the camera
          </h1>
          <form>
            <div class="my-6">
              <label class="label">
                Select image size
              </label>
              <input type="range" min="0" max="2" value="0" class="range range-sm px-2" step="1" />
              <div class="w-full flex justify-between text-xs font-bold my-4">
                <span>480x320</span>
                <span>640x480</span>
                <span>800x600</span>
              </div>
            </div>
            <div class="my-6">
              <label class="label cursor-pointer">
                <span class="label-text">RGB</span>
                <input type="checkbox" checked=true class="toggle toggle-sm" />
              </label>
            </div>
            <div class="my-6">
              <label class="label cursor-pointer">
                <span class="label-text">Show probabilities</span>
                <input type="checkbox" checked=true class="toggle toggle-sm" />
              </label>
            </div>
            <div>
              <button class="btn btn-active btn-primary">Reconfigure</button>
            </div>
          </form>
        </div>
      </div>
    </div>
    <div class="navbar-center">
      <p class="text-3xl font-bold">Sign Language Classifier</p>
    </div>
    <div class="navbar-end" />
  </div>
</template>
