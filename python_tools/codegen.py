import os
import shutil
import uuid
from . import TMP_FILE_OUTPUT 


class TmpFileSession(object):
    def __init__(self, obj):
        self._path = (TMP_FILE_OUTPUT / f"{id(obj)}").resolve()
    def get(self): 
        if not self._path.exists():
            os.makedirs(self._path)
        unique_id = str(uuid.uuid4())
        return self._path / f"{unique_id}.py"
    def close(self):
        
        if len(os.listdir(TMP_FILE_OUTPUT)) == 0:
            shutil.rmtree(TMP_FILE_OUTPUT, True)
        else:
            shutil.rmtree(self._path, True)
            

def tmp_file_session(obj):
    return TmpFileSession(obj)


def method_from_template(file_session: TmpFileSession, template: str, gl = {}, lo = {}):
    target_path = file_session.get()
    with open(target_path, 'w+') as f:
        f.write(template)
    code_compiled = compile(template, target_path, 'exec')
    exec(code_compiled, gl, lo)
    return lo

