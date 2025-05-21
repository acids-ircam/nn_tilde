import os
import importlib
import time
import sys
import pytest
from pathlib import Path

@pytest.fixture
def out_dir() -> Path:
    out_dir = (Path(__file__).parent / "model_out").resolve()
    if not out_dir.exists():
        os.makedirs(out_dir)
    return out_dir

@pytest.fixture
def test_name(request):
    return request.node.name

def import_code(code, glob, loc):
    outdir = Path('/tmp') / "nn_tilde" / "code" 
    os.makedirs(outdir, exist_ok=True)
    module_name = f"nntilde_tmp_{int(time.time())}"
    outpath = outdir / f"{module_name}.py"
    with open(outpath, "w+") as f:
        f.write(code)
       # Load the module
    spec = importlib.util.spec_from_file_location(module_name, outpath)
    module = importlib.util.module_from_spec(spec)
    exec(spec.loader.get_code(module_name), glob)
    loc.update(module.__dict__)
    


