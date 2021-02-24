//===========================================================================
//! @file   ModelMQO.cpp
//!	@brief  FQO���f��
//===========================================================================


//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
ModelMQO::ModelMQO()
{
}

//---------------------------------------------------------------------------
//! �f�X�g���N�^
//---------------------------------------------------------------------------
ModelMQO::~ModelMQO()
{
    for(auto& o : objects_) {
        delete o;
        o = nullptr;
    }

    for(auto& m : materials_) {
        delete m.texture_;
    }
}

//---------------------------------------------------------------------------
//! �ǂݍ���
//---------------------------------------------------------------------------
bool ModelMQO::load(const char* fileName, f32 scale)
{
    //----------------------------------------------------------
    // �t�@�C�����J��
    //----------------------------------------------------------
    FILE* fp;
    if(fopen_s(&fp, fileName, "rt") != 0) {
        return false;
    }

    //----------------------------------------------------------
    // ��s���t�@�C���I�[�܂ŉ�͂��Ă���
    //----------------------------------------------------------
    enum class State
    {
        Root,
        Object,
        Vertex,
        Face,
        Material,
    };

    State state = State::Root;   // ��Ԃ̏����l

    Object* object = nullptr;

    char str[1024];
    while(fgets(str, sizeof(str), fp)) {
        switch(state) {
            case State::Root:   //---- ���[�g��͒�
                if(strncmp(str, "Object ", 7) == 0) {
                    object = new Object();

                    // �I�u�W�F�N�g�������o�� Object "name"
                    char* p = strchr(str, '\"');
                    p += 1;   // ��؂�L�����X�L�b�v

                    char* name = p;   // ���O�擪

                    p  = strchr(p, '\"');
                    *p = 0;   // �����I�[

                    object->name_ = name;

                    state = State::Object;
                    break;
                }

                if(strncmp(str, "Material", 8) == 0) {
                    state = State::Material;
                    break;
                }

                break;
            case State::Object:   //---- �I�u�W�F�N�g��͒�
                if(strncmp(str, "}", 1) == 0) {
                    // �œK��
                    object->optimize();

                    // �I�u�W�F�N�g���X�g�ɂȂ�
                    objects_.push_back(object);
                    object = nullptr;

                    state = State::Root;
                    break;
                }
                if(strstr(str, "color ") != nullptr) {
                    Vector4 color;
                    sscanf_s(str, " color %f %f %f", &color.r_, &color.g_, &color.b_);
                    color.a_ = 1.0f;

                    object->color_ = color;

                    break;
                }

                if(strstr(str, "vertex ") != nullptr) {
                    state = State::Vertex;
                    break;
                }

                if(strstr(str, "facet ") != nullptr) {
                    f32 facet;
                    sscanf_s(str, " facet %f", &facet);
                    object->facet_ = facet;

                    break;
                }

                if(strstr(str, "face ") != nullptr) {
                    state = State::Face;
                    break;
                }

                break;
            case State::Vertex:   //---- ���_��͒�
                if(strstr(str, "}") != nullptr) {
                    state = State::Object;
                    break;
                }

                f32 x;
                f32 y;
                f32 z;

                sscanf_s(str, " %f %f %f", &x, &y, &z);

                x *= scale;
                y *= scale;
                z *= scale;

                object->vertices_.push_back(Vector3(x, y, z));

                break;

            case State::Face:   //---- �t�F�C�X��͒�
            {
                if(strstr(str, "}") != nullptr) {
                    state = State::Object;
                    break;
                }

                // �O�p�`���l�p�`�����`�F�b�N
                s32 vertexCount;   // 3 or 4
                sscanf_s(str, " %d", &vertexCount);

                if(vertexCount <= 2) {   // �s���Ȓl�̏ꍇ�̓X�L�b�v
                    break;
                }

                Face f;

                // �}�e���A���ԍ����擾
                f.material_ = -1;

                {
                    char* p = strstr(str, "M(");
                    if(p != nullptr) {
                        sscanf(p, "M(%d)", &f.material_);
                    }
                }

                if(vertexCount == 3) {
                    // �O�p�`�̏ꍇ
                    char* p = strstr(str, "V(");

                    s32 index[3];
                    sscanf(p, "V(%d %d %d)", &index[0], &index[1], &index[2]);

                    f32 u[3]{};
                    f32 v[3]{};

                    p = strstr(str, "UV(");
                    if(p) {
                        sscanf(p, "UV(%f %f %f %f %f %f)", &u[0], &v[0], &u[1], &v[1], &u[2], &v[2]);
                    }

                    f.vertexIndex_[0] = index[0];
                    f.vertexIndex_[1] = index[1];
                    f.vertexIndex_[2] = index[2];

                    const auto& v1         = object->vertices_[index[0]];
                    const auto& v2         = object->vertices_[index[1]];
                    const auto& v3         = object->vertices_[index[2]];
                    Vector3     faceNormal = Vector3::cross(v3 - v1, v2 - v1).normalize();
                    f.normal_[0]           = faceNormal;
                    f.normal_[1]           = faceNormal;
                    f.normal_[2]           = faceNormal;

                    f.u_[0] = u[0];
                    f.u_[1] = u[1];
                    f.u_[2] = u[2];

                    f.v_[0] = v[0];
                    f.v_[1] = v[1];
                    f.v_[2] = v[2];

                    // �O�p�`�̓o�^
                    object->faces_.push_back(f);
                }
                else {
                    // �l�p�`�̏ꍇ
                    char* p = strstr(str, "V(");

                    s32 index[4];
                    sscanf(p, "V(%d %d %d %d)", &index[0], &index[1], &index[2], &index[3]);

                    f32 u[4]{};
                    f32 v[4]{};

                    p = strstr(str, "UV(");
                    if(p) {
                        sscanf(p, "UV(%f %f %f %f %f %f %f %f)", &u[0], &v[0], &u[1], &v[1], &u[2], &v[2], &u[3], &v[3]);
                    }

                    f.vertexIndex_[0] = index[0];
                    f.vertexIndex_[1] = index[1];
                    f.vertexIndex_[2] = index[2];

                    const auto& v1         = object->vertices_[index[0]];
                    const auto& v2         = object->vertices_[index[1]];
                    const auto& v3         = object->vertices_[index[2]];
                    Vector3     faceNormal = Vector3::cross(v3 - v1, v2 - v1).normalize();
                    f.normal_[0]           = faceNormal;
                    f.normal_[1]           = faceNormal;
                    f.normal_[2]           = faceNormal;
                    f.normal_[3]           = faceNormal;

                    f.u_[0] = u[0];
                    f.u_[1] = u[1];
                    f.u_[2] = u[2];
                    f.v_[0] = v[0];
                    f.v_[1] = v[1];
                    f.v_[2] = v[2];

                    object->faces_.push_back(f);

                    f.vertexIndex_[0] = index[2];
                    f.vertexIndex_[1] = index[3];
                    f.vertexIndex_[2] = index[0];

                    f.u_[0] = u[2];
                    f.u_[1] = u[3];
                    f.u_[2] = u[0];
                    f.v_[0] = v[2];
                    f.v_[1] = v[3];
                    f.v_[2] = v[0];

                    object->faces_.push_back(f);
                }

                break;
            }
            case State::Material: {
                if(str[0] == '}') {
                    state = State::Root;
                    break;
                }

                // col()�\�������o
                Vector4 color(1, 1, 1, 1);
                {
                    char* p = strstr(str, "col(");
                    if(p) {
                        sscanf(p, "col(%f %f %f %f)", &color.r_, &color.g_, &color.b_, &color.a_);
                    }
                }
                gpu::Texture* texture = nullptr;

                // tex("")�\�������o
                char* p = strstr(str, "tex(");
                if(p != nullptr) {
                    // tex(" ��5�������i�߂�
                    p += 5;

                    // �e�N�X�`�����𒊏o
                    char* textureName = p;   // �擪��ۑ�
                    while(*p != '\"') {
                        p++;
                    }

                    *p = 0;

                    // �e�N�X�`����ǂݍ���

                    // MQO�t�@�C���̃p�X�w��ӏ��̃p�X���̂ݒ��o
                    char drive[MAX_PATH];
                    char path[MAX_PATH];
                    char file[MAX_PATH];
                    char ext[MAX_PATH];
                    _splitpath_s(fileName, drive, path, file, ext);

                    char textureFullPath[MAX_PATH];
                    strcpy(textureFullPath, drive);
                    strcat(textureFullPath, path);
                    strcat(textureFullPath, textureName);

                    texture = gpu::createTexture(textureFullPath);
                    if(!texture) {
                        MessageBox(nullptr, textureFullPath, "�e�N�X�`���ǂݍ��݂Ɏ��s���܂���.", MB_OK);
                        delete texture;
                        texture = nullptr;
                    }
                }

                // �}�e���A����o�^
                Material m;
                m.texture_ = texture;
                m.color_   = color;
                materials_.push_back(m);
                break;
            }
        }

        //		MessageBox(nullptr, str, "�ǂݍ���", MB_OK);
    }

    //----------------------------------------------------------
    // �t�@�C�������
    //----------------------------------------------------------
    fclose(fp);

    shaderVs_ = gpu::ShaderVs::create("shader/vsModel.fx");
    shaderPs_ = gpu::ShaderPs::create("shader/psModel.fx");

    return true;
}

//---------------------------------------------------------------------------
//! �`��
//---------------------------------------------------------------------------
void ModelMQO::render()
{
	gpu::setTexture(2, nullptr);	// ���t�l�X
	gpu::setTexture(3, nullptr);	// ���^���l�X

    //-------------------------------------------------------------
    // �_�Q�̕`��
    //-------------------------------------------------------------
#if 0
	glPointSize(3.0f);
	glColor3ub(255, 255, 255);

	glBegin(GL_POINTS);

		// �S�ẴI�u�W�F�N�g���i
		for( auto& o : _objects ) {

			// �I�u�W�F�N�g���̂��ׂĂ̒��_
			for( auto& v : o->_vertices ) {
				glVertex3fv((GLfloat*)&v);
			} 
		}

	glEnd();

	glPointSize(1.0f);
#endif
    //-------------------------------------------------------------
    // ���C���[�t���[���̕`��
    //-------------------------------------------------------------
#if 0
	glColor3ub(255, 255, 255);

	glBegin(GL_LINES);

		// �S�ẴI�u�W�F�N�g���i
		for( auto& o : _objects ) {
			auto&	v = o->_vertices;	// ���_�z��  

			// �S�Ẵt�F�C�X
			for( auto& face : o->_faces ) {
					glVertex3fv((GLfloat*)&v[ face._vertexIndex[0] ]);
					glVertex3fv((GLfloat*)&v[ face._vertexIndex[1] ]);

					glVertex3fv((GLfloat*)&v[ face._vertexIndex[1] ]);
					glVertex3fv((GLfloat*)&v[ face._vertexIndex[2] ]);

					glVertex3fv((GLfloat*)&v[ face._vertexIndex[2] ]);
					glVertex3fv((GLfloat*)&v[ face._vertexIndex[0] ]);
			} 
		}

	glEnd();

	glColor3ub(128, 128, 128);
#endif
    //-------------------------------------------------------------
    // �O�p�`�̕`��
    //-------------------------------------------------------------
    s32 material = -1;   // ���݂̃}�e���A���ԍ�
    dxTexture(getSystemTexture(SYSTEM_TEXTURE_NULL_WHITE));
    gpu::setShader(shaderVs_, shaderPs_);   // �V�F�[�_�[

    u32 triangleCount = 0;

    // �|���S���I�t�Z�b�g�@�\�Ńf�v�X�����炷
    //   glEnable(GL_POLYGON_OFFSET_FILL);
    // glPolygonOffset(1.0f, 1.0f);

    // �A���t�@�e�X�g�L����
    //  glEnable(GL_ALPHA_TEST);
    // glAlphaFunc(GL_GREATER, 0.5f);   // ��r�����@A > 0.5�̂Ƃ��ɕ`�悷��

    dxBegin(PT_TRIANGLES);
    // �S�ẴI�u�W�F�N�g���i
    for(auto& o : objects_) {
        auto& v = o->vertices_;   // ���_�z��

        // �I�u�W�F�N�g�J���[
        dxColor4f(o->color_.r_, o->color_.g_, o->color_.b_, o->color_.a_);

        // �S�Ẵt�F�C�X
        for(auto& face : o->faces_) {
            // �}�e���A���̕ύX
            if(material != face.material_ ||
               triangleCount > 65536

            ) {
                dxEnd();   //--- ��������`��R�}���h�����

                // �e�N�X�`���̐ݒ�
                gpu::Texture* texture = nullptr;

                if(face.material_ != -1) {
                    // �e�N�X�`������
                    texture = materials_[face.material_].texture_;
                    // �}�e���A���J���[
                    dxColor4f(materials_[face.material_].color_.r_,
                              materials_[face.material_].color_.g_,
                              materials_[face.material_].color_.b_,
                              materials_[face.material_].color_.a_);
                }
                else {
                    // �I�u�W�F�N�g�J���[
                    dxColor4f(o->color_.r_, o->color_.g_, o->color_.b_, o->color_.a_);
                }
                gpu::setTexture(4, texture ? texture : getSystemTexture(SYSTEM_TEXTURE_NULL_WHITE));

                gpu::setShader(shaderVs_, shaderPs_);   // �V�F�[�_�[

                dxBegin(PT_TRIANGLES);   //--- �ēx�J���Ȃ���

                // ���݂̔ԍ����X�V
                material      = face.material_;
                triangleCount = 0;
            }

            const auto& v1 = v[face.vertexIndex_[0]];
            const auto& v2 = v[face.vertexIndex_[1]];
            const auto& v3 = v[face.vertexIndex_[2]];

            dxTexCoord2f(face.u_[0], face.v_[0]);
            dxNormal3fv(face.normal_[0]);
            dxVertex3fv(v1);

            dxTexCoord2f(face.u_[1], face.v_[1]);
            dxNormal3fv(face.normal_[1]);
            dxVertex3fv(v2);

            dxTexCoord2f(face.u_[2], face.v_[2]);
            dxNormal3fv(face.normal_[2]);
            dxVertex3fv(v3);
            triangleCount++;
        }
    }
    dxEnd();

    // �A���t�@�e�X�g������
    //    glDisable(GL_ALPHA_TEST);

    // �|���S���I�t�Z�b�g������
    // glDisable(GL_POLYGON_OFFSET_FILL);

    // �e�N�X�`�������Ƃɖ߂�
    dxTexture(nullptr);
}

int sortFunc(const void* a, const void* b)
{
    int m1 = ((ModelMQO::Face*)a)->material_;
    int m2 = ((ModelMQO::Face*)b)->material_;

    if(m1 < m2)
        return -1;
    if(m1 > m2)
        return +1;
    return 0;
}

//---------------------------------------------------------------------------
//	�œK��
//---------------------------------------------------------------------------
void ModelMQO::Object::optimize()
{
    // �}�e���A���P�ʂŃ\�[�g
    std::qsort(faces_.data(), faces_.size(), sizeof(ModelMQO::Face), &sortFunc);

    //-------------------------------------------------------------
    // �@��������
    //-------------------------------------------------------------
    struct Point
    {
        Vector3 vertex_ = Vector3(0, 0, 0);
        Vector3 normal_ = Vector3(0, 0, 0);
        u32     count_  = 0;
    };
    std::vector<Point> points;
    std::vector<u32>   pointIndices;

    for(auto& f : faces_) {
        for(u32 i = 0; i < 3; ++i) {
            bool found = false;
            for(u32 index = 0; index < points.size(); ++index) {
                auto& p = points[index];
                if((p.vertex_ - vertices_[f.vertexIndex_[i]]).lengthSq() > 0.01f) {
                    continue;
                }

                // �@���̈�v�x�`�F�b�N
                if(Vector3::dot(p.normal_.normalize(), f.normal_[i]) < cosf(facet_ * (3.1415 / 180.0))) {
                    continue;
                }

                // ���łɋ��L���_����������}�[�W
                p.normal_ += f.normal_[i];
                p.count_++;
                pointIndices.emplace_back(index);   // �C���f�b�N�X�ԍ�

                found = true;
                break;
            }

            // �V�K�o�^
            if(!found) {
                pointIndices.emplace_back(points.size());   // �C���f�b�N�X�ԍ�

                Point p;
                p.vertex_ = vertices_[f.vertexIndex_[i]];
                p.normal_ = f.normal_[i];
                p.count_  = 1;
                points.emplace_back(std::move(p));
            }
        }
    }
    // ���ω�
    for(auto& p : points) {
        p.normal_ /= p.count_;
    }

    // ���L�@���������߂�
    {
        u32 index = 0;
        for(auto& f : faces_) {
            bool found   = false;
            f.normal_[0] = points[pointIndices[index++]].normal_;
            f.normal_[1] = points[pointIndices[index++]].normal_;
            f.normal_[2] = points[pointIndices[index++]].normal_;
        }
    }
    int a = 0;
}
