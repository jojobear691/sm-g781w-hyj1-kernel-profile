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
