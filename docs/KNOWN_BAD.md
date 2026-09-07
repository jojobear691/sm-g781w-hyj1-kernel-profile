# Known-bad artifacts

Never identify a binary by its directory or filename alone.

| SHA-256 | Reason |
|---|---|
| `be35dd2c5e1fd67ec03d655a5f74f3fc89b34de7b193b89d181bb7f17ab02cbf` | Stored under an `r8q` path but embeds `pa3q-S938NKSUACZF1-root-umh` and the retired PSELECT route. |
| `f275cde4494c11da1629ee77f9ddcce618d5ae404691b28d21726144304fccd2` | Wrong-target S938 preload. |
| `133dbf8b56b69db7ea9b6fc9687ddc62730a084d1444c56fd19cf90e0a8b8d7a` | Confirmed scheduler/signal-return panic family. |
| `a0d661a9e63822b5ec2df26f1a22eaff7d0cb671c754f88684104ef895503c3b` | Confirmed stale-waiter multicast panic family. |

The previous mixed-device target archive is withdrawn. Renaming inherited
tuning constants does not validate them for this phone.

## Excluded private bundles

These local bundles are retained for evidence or recovery, but they are not
GitHub release inputs and must not be mistaken for the public profile:

| SHA-256 | Exclusion reason |
|---|---|
| `7b10474d9ebc6fcff3df3a69d1861cfd52510e76d914be6fd3aff07529801ddc` | Private payload-source archive; outside the public read-only scope. |
| `68a54b70a09eb209fda462c99dbdaee63d5d9bd54fc941bbd3240201a891c118` | Combined crash-log and payload archive; contains sensitive raw material. |
| `f398acf878588bae651b887abc07aaea032dfe909f3d71e9bb3cb1f3c8fe9d4b` | Raw crash-log archive; not sanitized for publication. |
| `5ba4f79d7768a9f534da06a87f829ba88f300abbc9f85eeb8ae8acd1a17b6d68` | Misleading “verified files” ZIP that mixes boot epochs; withdrawn. |

Hash-only indexing preserves identity without distributing these bundles.
