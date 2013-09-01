import Sound.OpenSoundControl
import Sound.OSC.FD

ip = "127.0.0.1"
port = 7777

test = do s <- openUDP ip port
          let m = Message "/test" [Int 8, Float 1.2, String "hello"]
          sendOSC s m
