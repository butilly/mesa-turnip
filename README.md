# mesa-turnip

GameNative's Turnip tree and its CI, in one repository. Turnip is Mesa's Vulkan driver for Qualcomm Adreno GPUs (`src/freedreno/vulkan`). This repository follows upstream Mesa `main`, keeps the GameNative patches on top, and builds AdrenoTools zips that you import into GameNative.

The upstream Mesa README is [README.rst](README.rst).

## Branches

- `turnip` (default): upstream Mesa plus our patches and CI. All work goes here through pull requests.
- `upstream-main`: a mirror of `https://gitlab.freedesktop.org/mesa/mesa.git` `main`. The sync workflow force-pushes it. Never commit to it.

## Files

- `ci/build.sh`: builds `libvulkan_freedreno.so` with the Android NDK and packs it with `meta.json` into a zip. By default it builds this checkout. Set `MESA_REPO` and/or `MESA_REF` to clone and build another tree, and `PATCHES` to apply patches.
- `patches/`: patch files that a build can apply. See [patches/README.md](patches/README.md).
- `variants/matrix.yml`: the community builds (tree, ref and patches for each GPU family). See [variants/README.md](variants/README.md).
- `.github/workflows/turnip.yml`: the build workflow.
- `.github/workflows/upstream-sync.yml`: runs every day. It mirrors upstream `main` to `upstream-main` and opens or updates the pull request "Sync with upstream Mesa main". Merge that pull request with a merge commit, not squash or rebase.

## Pull request builds

Each pull request to `turnip` builds the PR's tree. The artifact is `turnip-pr-<number>-<mesa version>-<short sha>`, and the driver shows in GameNative as `pr-<number>`.

1. Make a branch from `turnip`: `git checkout -b fix-something origin/turnip`.
2. Commit the change and open a pull request to `turnip`.
3. When the "Turnip" run is complete, download the artifact from the run page or with `gh run download -R butilly/mesa-turnip <run-id>`.
4. Merge the pull request when the driver works on the target devices.

## Building a variant or another tree

Start "Turnip" from the Actions tab, or with `gh workflow run turnip.yml -R butilly/mesa-turnip -f variant=a8xx-gen8`. Inputs:

- `variant`: a row `id` from `variants/matrix.yml`. Empty builds the `turnip` checkout.
- `mesa_repo`, `mesa_ref`: build this tree and ref instead (they also override the variant's values).
- `patches`: patch URLs or `patches/` paths, one per line. They replace the variant's patches.
- `variant_name`: the driver name in GameNative. The default is the variant, or `checkout`.

The artifact is `turnip-<variant>-<mesa version>-<short sha>`. A tag `v*` builds the tagged commit and attaches the zip to a GitHub release.

## Adding a patch

Commit Turnip changes to `turnip` through a pull request. For a patch that only some builds use, put the file in `patches/` and list it in the `patches` input or in a variant row.

## Importing into GameNative

The artifact download is a zip that contains the driver zip. Extract only the outer zip. Copy the inner zip (`libvulkan_freedreno.so` + `meta.json`) to the device. In GameNative, go to **Settings > Emulation > Driver Manager > Import ZIP from device**. Then select the driver in the game's or container's graphics driver settings.

## Local build

`ci/build.sh` needs git, curl, unzip, zip, meson, ninja, python3 (mako, pyyaml, packaging), flex, bison, glslangValidator and pkg-config. It downloads the NDK (`NDK_VERSION`, default `r28c`) into `ci/work/` and writes the zip to `ci/out/`.
