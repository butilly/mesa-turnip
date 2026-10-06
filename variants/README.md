# Variants

`matrix.yml` lists the Turnip builds that the community uses, and the GPUs and devices that each one is for. Start `.github/workflows/turnip.yml` with `variant=<id>` to build a row: it uses `source.repo`, `source.ref` and the `url` of each patch. A row that has a patch with no `url` fails unless you give the `patches` input.

Fields in each row:

- `id`: the variant name. This is also the `variant_name` for the build.
- `status`: `verified` if we built the row and tested the zip on a device; `documentation` if the row only records what the community builds. All rows are `documentation` now.
- `source.repo` and `source.ref`: the Mesa tree and the branch, tag or commit to build.
- `patches`: patches applied in order. Each one has a `name`, a `url` if we know it (`null` if not), and `notes`.
- `targets.gpus` and `targets.devices`: the hardware that the row is for.
- `zips`: the zip names that the row makes, if it makes more than one.

Before you set a row to `verified`, set each patch URL to a fixed commit (not a branch), and record the device and the game that you tested.
