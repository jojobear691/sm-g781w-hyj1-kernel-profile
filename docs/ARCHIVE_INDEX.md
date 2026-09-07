# Private analysis archive index

Updated: 2026-09-07

This page indexes the bounded, package-level HYJ1 `offline-analysis` corpus
retained in the owner-controlled workspace. Each digest is the SHA-256 of the
package's `SHA256SUMS` file, not a digest of the directory itself.

The packages and their contents are not redistributed here. This lets the
full research history be identified without publishing binaries, raw logs,
physical placement data, proprietary material, or exploit implementation
details. Older entries may be superseded by later decisions; consult
[RESEARCH_STATUS.md](RESEARCH_STATUS.md) for the current conclusion.

The inventory contains 37 HYJ1 packages. A different-device Galaxy S26 census
found beside the corpus is intentionally excluded.

On 2026-09-07, `sha256sum -c --quiet SHA256SUMS` passed inside all 37 indexed
packages (`count=37`, `fail=0`).

| Analysis package | `SHA256SUMS` SHA-256 |
|---|---|
| `2026-09-01-hyj1-alt-stack-writers` | `df75a470445e73967e01805315a7b2c0fe970f36f03556b191ab421a49323420` |
| `2026-09-01-hyj1-blocking-writer-census-v1` | `8de0fcefc48346186aff3b8d30a93a1ddfc39ab503358573871549692da814c8` |
| `2026-09-01-hyj1-combined-lifecycle-witness-v1` | `9262f7fb3fe16f870b7120c6ef3b8a560300dc39f5f749df735e43c236f02abf` |
| `2026-09-01-hyj1-compat-sendmsg-gate-v1` | `912be4d1b35a79c5b9eb3942e5763d4864c1258edab742d161a1258b181366a3` |
| `2026-09-01-hyj1-compat-sendmsg-witness-v1` | `ce5acfcfeb242746d76f80da1708dc92d45541e71634c91d7a4bca10ae255324` |
| `2026-09-01-hyj1-consumer-cleanup-choreography-v1` | `ed6d7158a5fba66cde16701c1c9bf6db0ffd1272a52ca5509e16f18c60e2adbc` |
| `2026-09-01-hyj1-elf-provenance-ledger-v1` | `308319a08d47f8cf934f5c0a879c31a75e51d68d695bfe8f09a1b5df63353d23` |
| `2026-09-01-hyj1-full-slab-collector` | `bc53a7c24f7888826f1967ab9539ed4cb1b07b4f9d79fa87981b9179ba88ad76` |
| `2026-09-01-hyj1-high-placement-v1` | `0f4f0ba097d9b689a3eae84109290d984951c8e04ef57d4a85e748a259578497` |
| `2026-09-01-hyj1-lifecycle-tu-split-v1` | `241f74eae7bc37ae3ef80c3c9e4d33c1d39e64d52786f171d8ee76cd83880f6c` |
| `2026-09-01-hyj1-pi-lifecycle-witness-v1` | `4b63eee0cffd56d30e543d65b7955b079b5d2b0b745f0cebb0e9450698afaca9` |
| `2026-09-01-hyj1-pi-self-clear-gate-v1` | `fedbda334f57f39c502e19fa826b742ce314bf95280bb89523137c8353b6c71d` |
| `2026-09-01-hyj1-reclaim-identity-checkpoint-v1` | `bc06687ff9805fe72d37ca32d6580b889f6326edc66a2a7ae961c6fdf7e157bd` |
| `2026-09-01-hyj1-recovery-readiness-v1` | `3de94995145938e5a892af3f624163e0b21cce74b7ae1fa22160762aab20c916` |
| `2026-09-01-hyj1-retained-lifecycle-integration-v2` | `db7c68d0f5b9aaf6d9e576a9e0aff323cb06cd8b34153ed93a80c2ab9d125a14` |
| `2026-09-01-hyj1-retained-route-controller-v1` | `9a5f49bc4a08ba27505396bd13a187b14107ef4c1ef5c2fab4cbce03a4850339` |
| `2026-09-01-hyj1-retained-source-failclosed-gate-v1` | `10fa2370dab6ec0edc354109d215e3560f77c151951deb56b314da5d655eeffe` |
| `2026-09-01-hyj1-retained-source-seam-gate-v1` | `4413c8c42516c067c4ed40d1503af27ca085ef9e1d81b8c333e450c6554cc9ee` |
| `2026-09-01-hyj1-source-isolated-route-v1` | `e526a70833587b62301fc96f4e35388f16965319fb118d5f370989daedff92c7` |
| `2026-09-01-hyj1-source-provenance-gate-v1` | `8d8360b89c17f8a3f828f955f4da1d1918535ff94e96d9c0e026f107d5738a0f` |
| `2026-09-01-hyj1-source-snapshot-v1` | `b4c7c902d6d632290938505c8173d768457207f46b6dcf1b6c9a1b3625506756` |
| `2026-09-01-hyj1-stack-lifetime-select-gate-v1` | `fc3120e8d952408a473b78c22f5fe0bfb533b301336c7fdc53755037f91c771c` |
| `2026-09-06-hyj1-compat32-host-closure-v1` | `b529aa2209870a46184af72f89a2842f27c6818f8ed83d5da8b1c644ff667a64` |
| `2026-09-06-hyj1-compat32-integration-blueprint-v1` | `f2d3a084d7ab92cb338c4d5a61adb3be0e4b4e0ebddbc8a886fb91fde1cc2fcb` |
| `2026-09-06-hyj1-compat32-lifecycle-seam-conformance-v1` | `8b54b0eadcd067f508bea65e50a48f9c90f1885208e7e0eb7fa4110b7aafd9fb` |
| `2026-09-06-hyj1-elf-linked-capability-matrix-v1` | `afb5bcd03af165c6f30df3517731d3edfbbb02ff22e63de4f087502a2e282fd6` |
| `2026-09-06-hyj1-live-readiness-decision-v1` | `92836b52071187165e9d0c14ed170afb96e1fe78d6960a98750b1436c7227a47` |
| `2026-09-06-hyj1-live-readiness-decision-v2` | `48b8549f41e2530d1f8e39435ee8fb0c9e9d0ba61cf89d2418b6b20d579ff7ce` |
| `2026-09-06-hyj1-owned-order3-observer-census-v2` | `40b52473af6393cdf935fed5cb3240701718a0aea27b0dcbb95305c272f2f758` |
| `2026-09-06-hyj1-owned-order3-observer-static-gate-v1` | `3c57873bd3798f6b97118fcb7eda0458aea727d52ab9cbeaef51cfa14cb4c610` |
| `2026-09-06-hyj1-qemu-no-shrink-control-v1` | `2d424cb8dd602145d19118c62a2a9789827b746bc7f857b1c97e9276d5f46500` |
| `2026-09-06-hyj1-qemu-reclaim-repeatability-v1` | `1a8fe12525b4e30e6ce78b50fba92db44c5439d9236a72896da51b013f4f6931` |
| `2026-09-06-hyj1-qemu-sham-delay-control-v1` | `54f12b7f81de74a26f5b77d7615ad9484d57d26d8cb87ecf7c6c1b8d1308d989` |
| `2026-09-06-hyj1-reclaim-identity-proof-v2` | `7641a22cae2dcdf44d6fefd2f2d3b4caadd12de8d97c900f723cbf93c361e5bc` |
| `2026-09-06-hyj1-reclaim-tracepoint-patch-v1` | `cc6cc7dfa9fac942792f669c548c056d15f465fa160525a2df29afcc64cf8047` |
| `2026-09-06-hyj1-typed-consumer-trace-contract-v1` | `0e33c7c5743b58f54cfe0f049d9222685b11cd8c5ab255894f97bb801a8b201d` |
| `2026-09-06-hyj1-unix-splice-witness-v1` | `906632f323b67a0403ead97307f59d66c3b3a99ca723564f2b1c101e0d78caaf` |

This bounded list records the authoritative analysis-package layer. It does
not claim that every historical scratch build, phone staging copy, or
superseded output archive is authoritative.
